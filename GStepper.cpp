#include "GStepper.h"
#include "Config.h"

GStepper::GStepper()
{
  _feedMode = MANUALLY_FEED_MODE;
  _feedAutoMode = AUTO_FEED_MODE_ASYNC;
  _handwheelFactor = 0.1;
  _handWheelSpeed = 0;

  setPositionToNull();
  setEndStopsToZero();
  resetPosition();

  pinMode(Z_STEP_PIN, OUTPUT);
  pinMode(Z_DIR_PIN, OUTPUT);
  pinMode(Z_ENABLE_PIN, OUTPUT);

  digitalWrite(Z_DIR_PIN, HIGH);

  disable();
}

void GStepper::resetPosition()
{
  _targetPos = _currentPos;
  _oldHandWheelPosition = distanceInHandWheelUnits(); //_oldHandWheelPosition = 0;
  _deltaTimeForNextStep = 0;
  setSpeed(0.0);
  _dir = DIRECTION_LEFT;
  _handWheelSpeed = 0;
  _delta = 0;
}

void GStepper::moveTo(long absolute)
{
  if (_targetPos != absolute)
  {
    noInterrupts();
    _targetPos = absolute;
    //Serial.println(_targetPos);
    interrupts();
  }
  //Serial.println(distanceToGo());
}

long GStepper::moveToWithHandWheel(long newHandWheelPosition)
{
  //float distance = zHandWheelCounter / 4;
  //float realDinstance = distance * zHandwheelFactor;
  //float stepsToGo =  realDinstance / MM_PER_Z_STEP;
  // or short variant:
  long fStepsToGo = (newHandWheelPosition * _handwheelFactor) / MM_PER_Z_STEP_HANDWHEEL;
  //Serial.println(fStepsToGo);

  if (isEndStopsSet() && fStepsToGo > _leftEndStop)
  {
    _handWheelSpeed = 0.0;
    return -1;
  }
  else if (isEndStopsSet() && fStepsToGo < _rightEndStop)
  {
    _handWheelSpeed = 0.0;
    return -1;
  }

  moveTo(fStepsToGo);
  return 0;
}

void GStepper::calculateSpeedForHandWheel(long newHandWheelPosition)
{
  long deltaHandWheelPosition = abs(newHandWheelPosition - _oldHandWheelPosition);
  long deltaStepperPosition = abs((distanceToGo() / _handwheelFactor) * MM_PER_Z_STEP_HANDWHEEL) ;
  if (deltaHandWheelPosition < deltaStepperPosition)
  {
    deltaHandWheelPosition = deltaStepperPosition;
  }

  float distanceForDelta =  (deltaHandWheelPosition * _handwheelFactor) / 4;
  _handWheelSpeed = distanceForDelta * 4; // mm per second
  if (_handWheelSpeed > 0)
  {
    _deltaTimeForNextStep = 1000000 / (_handWheelSpeed / MM_PER_Z_STEP); // steps per second
    Timer3.attachInterruptFunction(callbackTimer, _deltaTimeForNextStep);
  }
  else
  {
    Timer3.detachInterrupt();
    //Serial.println("H");
  }
  _oldHandWheelPosition = newHandWheelPosition;
}

/**
   speed - mm per second;
   ATTENTION! speed must be bigger on 100 from original value!
*/
void GStepper::setSpeed(float speed)
{
  if (_speed != speed)
  {
    _speed = speed;

    if (_dir > 0)
    {
      digitalWriteFast(Z_DIR_PIN, HIGH);
    }
    else
    {
      digitalWriteFast(Z_DIR_PIN, LOW);
    }
    //Serial.println(_speed);
    if (_speed > 0.0)
    {
      _deltaTimeForNextStep = 1000000.0 / (_speed / MM_PER_Z_STEP);
      //Serial.println(1000000.0 / (_speed / MM_PER_Z_STEP));
      //Serial.println( (long)(1000000.0 / (_speed / MM_PER_Z_STEP)));
      //Serial.println(speed );
      //Serial.println((speed * 100) / 625);
      //Serial.println(_deltaTimeForNextStep);
      //Timer3.initialize(_deltaTimeForNextStep);
      Timer3.attachInterruptFunction(callbackTimer, _deltaTimeForNextStep);
    }
    else
    {
      Timer3.detachInterrupt();
      //Serial.println("D");
    }
  }
}

void GStepper::run()
{
  //Serial.println(_targetPos - _currentPos);

  if (_targetPos == _currentPos)
    return;
  //Serial.println(_targetPos);

  if (distanceToGo() > 0)
  {
    digitalWriteFast(Z_DIR_PIN, HIGH); // pin 36
    _currentPos++;
  }
  else
  {
    digitalWriteFast(Z_DIR_PIN, LOW); // pin 36
    _currentPos--;
  }

  makeStep();

  //Serial.println(_currentTimeInterval);
}

/*
  void GStepper::run()
  {
  //Serial.println(_targetPos - _currentPos);

  if (_targetPos == _currentPos)
    return;
  //Serial.println(_targetPos);

  _nowTime = micros();
  if (_nowTime - _lastStepTime >= _deltaTimeForNextStep)
  {
    if (distanceToGo() > 0)
    {
      digitalWriteFast(Z_DIR_PIN, HIGH); // pin 36
      _currentPos++;
    }
    else
    {
      digitalWriteFast(Z_DIR_PIN, LOW); // pin 36
      _currentPos--;
    }

    makeStep();

    _lastStepTime = _nowTime;
  }
  //Serial.println(_currentTimeInterval);
  }*/

void GStepper::runSpeed()
{
  if (_dir > 0)
  {
    _currentPos++;
  }
  else
  {
    _currentPos--;
  }

  if (isEndStopsSet() && _currentPos > _leftEndStop)
  {
    _currentPos = _leftEndStop;
  }
  else if (isEndStopsSet() && _currentPos < _rightEndStop)
  {
    _currentPos = _rightEndStop;
  }
  else
  {
    makeStep();
  }
}

/*
  void GStepper::runSpeed()
  {
  _nowTime = micros();
  if (_nowTime - _lastStepTime >= _deltaTimeForNextStep && _speed != 0)
  {
     if (_dir > 0)
    {
      _currentPos++;
    }
    else
    {
      _currentPos--;
    }

    if (isEndStopsSet() && _currentPos > _leftEndStop)
    {
      _currentPos = _leftEndStop;
    }
    else if (isEndStopsSet() && _currentPos < _rightEndStop)
    {
      _currentPos = _rightEndStop;
    }
    else
    {
      makeStep();
    }
    _lastStepTime = _nowTime;
  }
  }*/

void GStepper::makeStep()
{
  digitalWriteFast(Z_STEP_PIN, HIGH);
  delayMicroseconds(5);
  digitalWriteFast(Z_STEP_PIN, LOW);
}

void GStepper::setPositionToNull()
{
  _targetPos = 0;
  _currentPos = 0;
}

void GStepper::disable()
{
  digitalWriteFast(Z_ENABLE_PIN, HIGH);
}

void GStepper::enable()
{
  digitalWriteFast(Z_ENABLE_PIN, LOW);
}

long GStepper::distanceToGo()
{
  return _targetPos - _currentPos;
}

float GStepper::getHandWheelSpeed()
{
  return _handWheelSpeed;
}

void GStepper::setHandWheelFactor(float newHandWheelFactor)
{
  _handwheelFactor = newHandWheelFactor;
  resetPosition();
}

float GStepper::distanceInMM()
{
  return _currentPos * MM_PER_Z_STEP;
}

long GStepper::distanceInHandWheelUnits()
{
  return (_currentPos * MM_PER_Z_STEP_HANDWHEEL) / _handwheelFactor;
}

volatile long GStepper::getCurrentPosition()
{
  return _currentPos;
}

void GStepper::setLeftEndStopInSteps(long leftStopInSteps)
{
  _leftEndStop =  leftStopInSteps;
}

void GStepper::setRightEndStopInSteps(long rightStopInSteps)
{
  _rightEndStop = rightStopInSteps;
}

long GStepper::getLeftEndStopInSteps()
{
  return _leftEndStop;
}

float GStepper::getLeftEndStopInMM()
{
  return (float)_leftEndStop * MM_PER_Z_STEP;
}

float GStepper::getRightEndStopInMM()
{
  return (float)_rightEndStop * MM_PER_Z_STEP;
}

long GStepper::getRightEndStopInSteps()
{
  return _rightEndStop;
}

void GStepper::setEndStopsToZero()
{
  _leftEndStop = 0L;
  _rightEndStop = 0L;
}

void GStepper::setCurrentPositionToLeftEndStop()
{
  _leftEndStop = _currentPos;
}

void GStepper::setCurrentPositionToRightEndStop()
{
  _rightEndStop = _currentPos;
}

boolean GStepper::isEndStopsSet()
{
  return (_leftEndStop != 0 || _rightEndStop != 0);
}

void GStepper::increaseAutoSpeed()
{
  _autoSpeedCount ++;
  if (_autoSpeedCount >= NUM_AUTO_SPEEDS)
  {
    _autoSpeedCount = 0;
  }
  _oldAutoSpeedCount = _autoSpeedCount;
}

void GStepper::decreaseAutoSpeed()
{
  _autoSpeedCount --;
  if (_autoSpeedCount < 0)
  {
    _autoSpeedCount = NUM_AUTO_SPEEDS - 1;
  }
  _oldAutoSpeedCount = _autoSpeedCount;
}

void GStepper::quickAutoSpeed()
{
  if (!_isQuickAutoSpeed)
  {
    _oldAutoSpeedCount = _autoSpeedCount;
    _autoSpeedCount = QUICK_AUTO_SPEED_POSITION;
    _isQuickAutoSpeed = true;
  }
  //Serial.println(_oldAutoSpeedCount);
  //Serial.println(_autoSpeedCount);
}

void GStepper::backToNormalAutoSpeed()
{
  if (_isQuickAutoSpeed)
  {
    _autoSpeedCount = _oldAutoSpeedCount;
    _isQuickAutoSpeed = false;
  }
}

void GStepper::increaseAutoSyncSpeed()
{
  _autoSyncSpeedCount ++;
  if (_autoSyncSpeedCount >= NUM_AUTO_SPEEDS_SYNC)
    _autoSyncSpeedCount = 0;

}

void GStepper::decreaseAutoSyncSpeed()
{
  _autoSyncSpeedCount --;
  if (_autoSyncSpeedCount < 0)
  {
    _autoSyncSpeedCount = NUM_AUTO_SPEEDS_SYNC - 1;
  }
}

float GStepper::getAutoSpeed()
{
  return _zAutoFeedSpeeds[_autoSpeedCount];
}

float GStepper::getAutoSyncSpeed()
{
  return _zAutoFeedSyncSpeeds[_autoSyncSpeedCount];
}

byte GStepper::getFeedMode()
{
  return _feedMode;
}

void GStepper::setFeedMode(byte feedMode)
{
  _feedMode = feedMode;
}

byte GStepper::getFeedAutoMode()
{
  return _feedAutoMode;
}

void GStepper::setFeedAutoMode(byte feedAutoMode)
{
  _feedAutoMode = feedAutoMode;
}

void GStepper::changeFeedAutoMode()
{
  if ( _feedAutoMode == MANUALLY_FEED_MODE)
    _feedAutoMode = AUTO_FEED_MODE;
  else
    _feedAutoMode = MANUALLY_FEED_MODE;
}

void GStepper::setDirection(char direction)
{
  _dir = direction;
}

char GStepper::getDirection()
{
  return _dir;
}

