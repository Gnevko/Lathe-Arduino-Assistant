#ifndef GJOYSTICK_H
#define GJOYSTICK_H

#include "Config.h"
#include "GHandWheel.h"
#include "GDisplay.h"
#include "GStepper.h"
#include "GSpindle.h"

#include <stdlib.h>
#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#include <wiring.h>
#endif

extern Encoder zHandWheelEncoder;
extern GSpindle spindle;
extern GHandWheel zHandWheel;
extern GDisplay display;

class GJoystick
{
  public:
    GJoystick();
    void checkCurrentJoystickPosition(unsigned long timeNow);
    void validateJoystickPosition();
    void initJoystick();

  private:
    // a variable for the Joystick Position: 0 - neutral; 1 - Z-Left/Forward; 2 - Z-Right/Backward; 3 - X-Forward; 4 - X-Backward
    int _joystickPosition ;
    unsigned long _lastJoysticChanges;
};

#endif // GJOYSTICK_H


