#include "GHandWheel.h"

GHandWheel::GHandWheel()
{
  _timeOld = 0;
  _handWheelOldCounter = 0;
  _isHandWheelRunning = false;
  _handWheelFactorCount = 0;
}

void GHandWheel::initHandWheel(GDisplay lcd)
{
  lcd.displayHandWheelFactor(_handWheelFactros[_handWheelFactorCount]);
}

void GHandWheel::increaseSpeed()
{
  _handWheelFactorCount ++;
  if (_handWheelFactorCount >= NUM_HANDWHEELFACTORSS)
  {
    _handWheelFactorCount = 0;
  }
}

void GHandWheel::decreaseSpeed()
{
  _handWheelFactorCount --;
  if (_handWheelFactorCount < 0)
  {
    _handWheelFactorCount = NUM_HANDWHEELFACTORSS - 1;
  }
}

float GHandWheel::getSpeed()
{
  return _handWheelFactros[_handWheelFactorCount];
}

void GHandWheel::handleUpdate(unsigned long timeNow, long handWheelCounter)
{
  //waiting for Z-Handwheel Impulses
  //Serial.println(_handWheelCounter);
  if (timeNow - _timeOld > 250)
  {
    zStepper.calculateSpeedForHandWheel(handWheelCounter);
    if (zStepper.getHandWheelSpeed() == 0)
    {
      _isHandWheelRunning = false;
    }
    _timeOld = timeNow;
  }
  if (_handWheelOldCounter != handWheelCounter)
  {
    if (_isHandWheelRunning == false)
    {
      _isHandWheelRunning = true;
      _timeOld = timeNow;
    }
    //Serial.println(handWheelCounter);
    if (zStepper.moveToWithHandWheel(handWheelCounter) == -1)
    {
      zHandWheelEncoder.write(_handWheelOldCounter);
    }
  }
  _handWheelOldCounter = handWheelCounter;
}

