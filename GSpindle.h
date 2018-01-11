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
    unsigned int calculateSpindleRevolutionsPerMinute();
    boolean isReady();
    void reset();
    unsigned int calculateMMPerSpindleRevolutionPerSecond(int zAutoFeedSyncSpeed);

  protected:
    unsigned int calculateSpindleRevolutionsPerSecond();
    
  private:
    volatile unsigned long _spindleUpdateTimePhase;
    volatile unsigned long _spindleDeltaPhase;
    volatile boolean _firstRevilution;
    volatile byte _spindlePhaseCounter;
    volatile unsigned int _spindleRevolutionsPerSecond;
};

#endif //GSPINDLE_H




