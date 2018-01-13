#include "GSpindle.h"

GSpindle::GSpindle(int pin)
{
  pinMode(pin, INPUT);
  digitalWrite(pin, HIGH);

  _spindleUpdateTimePhase = 0;
  _spindleDeltaPhase = 0;
  _firstRevilution = true;
  _spindlePhaseCounter = 0;
  _spindleRevolutionsPerSecond = 0;
}

void GSpindle::callbackSpindleUpdate()
{
  _spindleDeltaPhase = micros() - _spindleUpdateTimePhase;
  Serial.println(_spindleDeltaPhase);
  _spindleUpdateTimePhase = micros();
  if (isReady()) {
    _spindleRevolutionsPerSecond = calculateSpindleRevolutionsPerSecond();
    //Serial.println(_spindleRevolutionsPerSecond);
  }

  _firstRevilution = false;
}

/*

*/
float GSpindle::calculateSpindleRevolutionsPerSecond()
{
  if (!_firstRevilution && _spindleUpdateTimePhase != 0)
  {
    return  1000000.0 / _spindleDeltaPhase;
  }
  else
    return 0;
}

/*
   
*/
float GSpindle::calculateMMPerSpindleRevolutionPerSecond(int zAutoFeedSyncSpeed)
{
  return _spindleRevolutionsPerSecond * zAutoFeedSyncSpeed;
}

/*
   return integer value for the spindle speed: revolutions per minute
*/
float GSpindle::calculateSpindleRevolutionsPerMinute()
{
  return _spindleRevolutionsPerSecond * 60.0;
}

boolean GSpindle::isReady()
{
  return  !_firstRevilution;
}

void GSpindle::reset()
{
  _firstRevilution = true;
}
