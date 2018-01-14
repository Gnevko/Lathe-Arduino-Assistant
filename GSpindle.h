#ifndef GSPINDLE_H
#define GSPINDLE_H
#include "Config.h"

#include <stdlib.h>
#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#include <wiring.h>
#endif

class GSpindle
{
  public:
    GSpindle(int pin);
    void callbackSpindleUpdate();
    float calculateSpindleRevolutionsPerMinute();
    boolean isReady();
    void reset();
    float calculateMMPerSpindleRevolutionPerSecond(float zAutoFeedSyncSpeed);

  protected:
    float calculateSpindleRevolutionsPerSecond();
    
  private:
    volatile unsigned long _spindleUpdateTimePhase;
    volatile unsigned long _spindleDeltaPhase;
    volatile boolean _firstRevilution;
    volatile byte _spindlePhaseCounter;
    volatile float _spindleRevolutionsPerSecond;
};

#endif //GSPINDLE_H




