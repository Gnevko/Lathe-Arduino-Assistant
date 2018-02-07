#ifndef GDISPLAY_H
#define GDISPLAY_H

#include "Config.h"

#include <stdlib.h>
#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#include <wiring.h>
#endif

class GDisplay
{
  public:
    GDisplay(LiquidCrystal_I2C * lcd);
    void displayMode (byte zFeedMode, byte zFeedAutoMode, int zFeedDir);
    void displayDistance(float distance);
    void displaySelectMode (byte zFeedAutoMode);
    void displaySpindleSpeed(float _speedToShow);
    void displayHandWheelFactor(float _handWheelFactor);
    void displayAutoSpeed(float _autoSpeed);
    void displaySyncSpeed(float _autoSyncSpeed);
    void displayLeftEndStop(float _leftStop, boolean isEnabled);
    void displayRightEndStop(float _rightStop, boolean isEnabled);
    void displayMenuMark(int menuMode);

  protected:
    void displayNumber(float numberToDisplay, int cursorPos, int rowPos);

  private:
    LiquidCrystal_I2C * _lcd;
};

#endif // GDISPLAY_H
