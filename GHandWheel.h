#ifndef GHANDWHEEL_H
#define GHANDWHEEL_H

#include "Config.h"
#include "GDisplay.h"
#include "GStepper.h"

#include <stdlib.h>
#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#include <wiring.h>
#endif

#define NUM_HANDWHEELFACTORSS 4

extern Encoder zHandWheelEncoder;
extern GStepper zStepper;

class GHandWheel
{
  public:
    GHandWheel();
    void initHandWheel(GDisplay lcd);
    void increaseSpeed();
    void decreaseSpeed();
    float getSpeed();
    void handleUpdate(unsigned long timeNow, long handWheelCounter);

  private:
    unsigned long _timeOld;
    long _handWheelOldCounter;
    boolean _isHandWheelRunning;
    int _handWheelFactorCount;
    const float _handWheelFactros[NUM_HANDWHEELFACTORSS] = {0.01, 0.02, 0.05, 0.1};
};

#endif //GHANDWHEEL_H
