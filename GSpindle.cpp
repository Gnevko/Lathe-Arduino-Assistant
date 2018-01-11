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
  _spindleUpdateTimePhase = micros();
  if (!_firstRevilution) {
    _spindleRevolutionsPerSecond = calculateSpindleRevolutionsPerSecond();
  }

  if (_spindlePhaseCounter == 0)
  {
    _spindlePhaseCounter = 1;
  }
  else if (_spindlePhaseCounter == 1)
  {
    _spindlePhaseCounter = 0;
    _firstRevilution = false;
  }
}

/*
   ATTENTION! multiplay 100 for precision: 1000000 * 100 / spindleDeltaPhase
*/
unsigned int GSpindle::calculateSpindleRevolutionsPerSecond()
{
  if (!_firstRevilution && _spindleUpdateTimePhase != 0)
  {
    return 100000000 / _spindleDeltaPhase; // multiplay 100 for precision 1000000 * 100 / spindleDeltaPhase
  }
  else
    return 0;
}

/*
    ATTENTION! Return value is reather on 10000 as it should be:
    spindleRevolutionsPerSecond and zAutoFeedSyncSpeeds are already multiplayed by 100 for better precision.
*/
unsigned int GSpindle::calculateMMPerSpindleRevolutionPerSecond(int zAutoFeedSyncSpeed)
{
  return _spindleRevolutionsPerSecond * zAutoFeedSyncSpeed;
}

/*
   return integer value for the spindle speed: revolutions per minute
*/
unsigned int GSpindle::calculateSpindleRevolutionsPerMinute()
{
  return _spindleRevolutionsPerSecond * 60 / 100;
}

boolean GSpindle::isReady()
{
  return  !_firstRevilution;
}

void GSpindle::reset()
{
  _firstRevilution = true;
}
