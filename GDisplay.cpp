#include "GDisplay.h"

GDisplay::GDisplay(LiquidCrystal_I2C * lcd)
{
  _lcd  = lcd;
}

/*
     Neutral ----------
   << Auto >>----------
   <<Thread>>----------
   --------------------
   --------------------
   --------------------
*/
void GDisplay::displayMode (byte zFeedMode, byte zFeedAutoMode, int zFeedDir)
{
  _lcd->setCursor(0, 0);
  _lcd->print(F("           "));
  _lcd->setCursor(0, 0);

  if (zFeedMode == 0)
  {
    _lcd->print(F("  Neutral "));
  }
  else if (zFeedMode == 1)
  {
    if (zFeedAutoMode == 0)
    {
      if (zFeedDir == 1)
      {
        _lcd->print(F("<< Auto   "));
      }
      else
      {
        _lcd->print(F("   Auto >>"));
      }
    }
    else if (zFeedAutoMode == 1)
    {
      if (zFeedDir == 1)
      {
        _lcd->print(F("<<Thread  "));
      }
      else
      {
        _lcd->print(F("  Thread>>"));
      }
    }
  }
}

/*
      AUTO   ----------
     THREAD  ----------
   --------------------
   --------------------
   --------------------
*/
void GDisplay::displaySelectMode (byte zFeedAutoMode)
{
  _lcd->setCursor(0, 0);
  _lcd->print(F("           "));
  _lcd->setCursor(0, 0);

  if (zFeedAutoMode == 0)
  {
    _lcd->print(F("   AUTO   "));
  }
  if (zFeedAutoMode == 1)
  {
    _lcd->print(F("  THREAD  "));
  }
}

/*
   ----------Z: -130.00
   --------------------
   --------------------
   --------------------
*/
void GDisplay::displayDistance(float distance)
{
  _lcd->setCursor(11, 0);
  _lcd->print(F("Z:"));
  displayNumber(distance, 13, 0);
}

void GDisplay::displayMenuMark(int menuMode)
{
  if (menuMode == MENU_HANDWHEELFACTOR)
  {
    _lcd->setCursor(0, 2);
    _lcd->print(F(">"));
  }
  else
  {
    _lcd->setCursor(0, 2);
    _lcd->print(F(" "));
  }

  if (menuMode == MENU_Z_AUTOFEEDSPEED)
  {
    _lcd->setCursor(7, 2);
    _lcd->print(F(">"));
  }
  else
  {
    _lcd->setCursor(7, 2);
    _lcd->print(F(" "));
  }

  if (menuMode == MENU_Z_AUTOFEEDSPEEDSYNC)
  {
    _lcd->setCursor(14, 2);
    _lcd->print(F(">"));
  }
  else
  {
    _lcd->setCursor(14, 2);
    _lcd->print(F(" "));
  }

  if (menuMode == MENU_Z_LEFT_STOP)
  {
    _lcd->setCursor(0, 3);
    _lcd->print(F(">"));
  }
  else
  {
    _lcd->setCursor(0, 3);
    _lcd->print(F(" "));
  }

  if (menuMode == MENU_Z_RIGHT_STOP)
  {
    _lcd->setCursor(11, 3);
    _lcd->print(F(">"));
  }
  else
  {
    _lcd->setCursor(11, 3);
    _lcd->print(F(" "));
  }
}

/*
   --------------------
    S:  130.00---------
   --------------------
   --------------------
*/
void GDisplay::displaySpindleSpeed(float _speedToShow)
{
  _lcd->setCursor(0, 1);
  _lcd->print(F(" S:"));
  displayNumber(_speedToShow, 3, 1);
}

/*
   --------------------
   --------------------
   >F0.01--------------
   --------------------
*/
void GDisplay::displayHandWheelFactor(float _handWheelFactor)
{
  _lcd->setCursor(1, 2);
  _lcd->print(F("F"));
  _lcd->print(_handWheelFactor);
}

/*
   --------------------
   --------------------
   ------>A0.08--------
   --------------------
*/
void GDisplay::displayAutoFeedSpeed(int _autoSpeed)
{
  _lcd->setCursor(8, 2);
  _lcd->print(F("A"));
  _lcd->print(_autoSpeed / 100.0);
}

/*
   --------------------
   --------------------
   ------------>S0.08--
   --------------------
*/
void GDisplay::displayAutoFeedSyncSpeed(int _autoSyncSpeed)
{
  _lcd->setCursor(15, 2);
  _lcd->print(F("S"));
  _lcd->print(_autoSyncSpeed / 100.0);
}

/*
   --------------------
   --------------------
   --------------------
   >L 123.00-----------
*/
void GDisplay::displayLeftEndStop(float _leftStop)
{
  _lcd->setCursor(1, 3);
  _lcd->print(F("L"));
  displayNumber(_leftStop, 2, 3);
}

/*
   --------------------
   --------------------
   --------------------
   ---------->R-123.00-
*/
void GDisplay::displayRightEndStop(float _rightStop)
{
  _lcd->setCursor(12, 3);
  _lcd->print(F("R"));
  displayNumber(_rightStop, 13, 3);
}

void GDisplay::displayNumber(float numberToDisplay, int cursorPos, int rowPos)
{
  if (numberToDisplay <= 9.99 && numberToDisplay >= 0)
  {
    _lcd->print(F("   "));
    _lcd->setCursor(cursorPos + 3, rowPos);
  }
  else if (numberToDisplay < 100.0 && numberToDisplay >= 0)
  {
    _lcd->print(F("  "));
    _lcd->setCursor(cursorPos + 2, rowPos);
  }
  else if (numberToDisplay < 1000.0 && numberToDisplay >= 0)
  {
    _lcd->print(F(" "));
    _lcd->setCursor(cursorPos + 1, rowPos);
  }
  else if (numberToDisplay >= -9.99 && numberToDisplay < 0)
  {
    _lcd->print(F("  "));
    _lcd->setCursor(cursorPos + 2, rowPos);
  }
  else if (numberToDisplay >= -99.99 && numberToDisplay < 0)
  {
    _lcd->print(F(" "));
    _lcd->setCursor(cursorPos + 1, rowPos);
  }
  else if (numberToDisplay >= -999.99 && numberToDisplay < 0)
  {
    _lcd->setCursor(cursorPos, rowPos);
  }

  _lcd->print(numberToDisplay);
}
