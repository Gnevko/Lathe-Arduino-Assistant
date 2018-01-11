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
    void displayAutoFeedSpeed(int _autoSpeed);
    void displayAutoFeedSyncSpeed(int _autoSyncSpeed);
    void displayLeftEndStop(float _leftStop);
    void displayRightEndStop(float _rightStop);
    void displayMenuMark(int menuMode);

  protected:
    void displayNumber(float numberToDisplay, int cursorPos, int rowPos);

  private:
    LiquidCrystal_I2C * _lcd;
};

#endif // GDISPLAY_H
