#include "GMenu.h"


GMenu::GMenu()
{
  _menuMode = 1;
  _lastMenuChanges = -60;
  _menuOldCounter = 0;
}

void GMenu::checkMenu(unsigned long timeNow, int menuCounter)
{
  if (timeNow - _lastMenuChanges > MENU_REACTION_TIME)
  {
    //Serial.println(menuCounter);
    if (menuCounter % 4 == 0)
    {
      if (menuCounter > _menuOldCounter)
      {
        //Hand wheel mode
        if (_menuMode == MENU_HANDWHEELFACTOR)
        {
          zHandWheel.increaseSpeed();

          noInterrupts();
          zStepper.setHandWheelFactor(zHandWheel.getSpeed());
          zStepper.resetPosition();
          zHandWheelEncoder.write(zStepper.distanceInHandWheelUnits());
          interrupts();
          display.displayHandWheelFactor(zHandWheel.getSpeed());
        }
        // auto feed mode
        else if (_menuMode == MENU_Z_AUTOFEEDSPEED)
        {
          zStepper.increaseAutoSpeed();

          noInterrupts();
          zStepper.resetPosition();
          zHandWheelEncoder.write(zStepper.distanceInHandWheelUnits());
          interrupts();
          display.displayAutoSpeed(zStepper.getAutoSpeed());
        }
        // auto sync feed mode
        else if (_menuMode == MENU_Z_AUTOFEEDSPEEDSYNC)
        {
          zStepper.increaseSyncSpeed();
          noInterrupts();
          zStepper.resetPosition();
          zHandWheelEncoder.write(zStepper.distanceInHandWheelUnits());
          interrupts();
          display.displaySyncSpeed(zStepper.getSyncSpeed());
        }
        // set left stop
        else if (_menuMode == MENU_Z_LEFT_STOP)
        {
          noInterrupts();
          zStepper.setLeftEndStopInSteps(zStepper.getLeftEndStopInSteps() + (long) (0.1 / MM_PER_Z_STEP));
          interrupts();
          display.displayLeftEndStop(zStepper.getLeftEndStopInMM(), zStepper.isLeftEndStopSet());
        }
        // set right stop
        else if (_menuMode == MENU_Z_RIGHT_STOP)
        {
          noInterrupts();
          zStepper.setRightEndStopInSteps(zStepper.getRightEndStopInSteps() + (long) (0.1 / MM_PER_Z_STEP));
          if (zStepper.getRightEndStopInSteps() > 0)
            zStepper.setRightEndStopInSteps(0L);
          interrupts();
          display.displayRightEndStop(zStepper.getRightEndStopInMM(), zStepper.isRightEndStopSet());
        }
      }
      else if (menuCounter < _menuOldCounter)
      {
        if (_menuMode == MENU_HANDWHEELFACTOR)
        {
          zHandWheel.decreaseSpeed();
          noInterrupts();
          zStepper.setHandWheelFactor(zHandWheel.getSpeed());
          zStepper.resetPosition();
          zHandWheelEncoder.write(zStepper.distanceInHandWheelUnits());
          interrupts();
          display.displayHandWheelFactor(zHandWheel.getSpeed());
        }
        // auto feed mode
        else if (_menuMode == MENU_Z_AUTOFEEDSPEED)
        {
          zStepper.decreaseAutoSpeed();
          noInterrupts();
          zStepper.resetPosition();
          zHandWheelEncoder.write(zStepper.distanceInHandWheelUnits());
          interrupts();
          display.displayAutoSpeed(zStepper.getAutoSpeed());
        }
        // auto sync feed mode
        else if (_menuMode == MENU_Z_AUTOFEEDSPEEDSYNC)
        {
          zStepper.decreaseSyncSpeed();
          noInterrupts();
          zStepper.resetPosition();
          zHandWheelEncoder.write(zStepper.distanceInHandWheelUnits());
          interrupts();
          display.displaySyncSpeed(zStepper.getSyncSpeed());
        }
        // set left stop
        else if (_menuMode == MENU_Z_LEFT_STOP)
        {
          noInterrupts();
          zStepper.setLeftEndStopInSteps(zStepper.getLeftEndStopInSteps() - (long) (0.1 / MM_PER_Z_STEP));
          if (zStepper.getLeftEndStopInSteps() < 0)
            zStepper.setLeftEndStopInSteps(0L);
          interrupts();
          display.displayLeftEndStop(zStepper.getLeftEndStopInMM(), zStepper.isLeftEndStopSet());
        }
        // set right stop
        else if (_menuMode == MENU_Z_RIGHT_STOP)
        {
          noInterrupts();
          zStepper.setRightEndStopInSteps(zStepper.getRightEndStopInSteps() - (long) (0.1 / MM_PER_Z_STEP));
          interrupts();
          display.displayRightEndStop(zStepper.getRightEndStopInMM(), zStepper.isRightEndStopSet());
        }
      }
      display.displayMenuMark(_menuMode);
      _menuOldCounter = menuCounter;
      _lastMenuChanges = timeNow;
    }
  }
}

void GMenu::menuClick()
{
  if (_menuMode == MENU_HANDWHEELFACTOR)
  {
    _menuMode = MENU_Z_AUTOFEEDSPEED;
  }
  else if (_menuMode == MENU_Z_AUTOFEEDSPEED)
  {
    _menuMode = MENU_Z_AUTOFEEDSPEEDSYNC;
  }
  else if (_menuMode == MENU_Z_AUTOFEEDSPEEDSYNC)
  {
    _menuMode = MENU_Z_LEFT_STOP;
  }
  else if (_menuMode == MENU_Z_LEFT_STOP)
  {
    _menuMode = MENU_Z_RIGHT_STOP;
  }
  else
  {
    _menuMode = MENU_HANDWHEELFACTOR;
  }
  display.displayMenuMark(_menuMode);
}

void GMenu::menuDoubleClick()
{
  if (_menuMode == MENU_Z_LEFT_STOP)
  {
    zStepper.setCurrentPositionToLeftEndStop();
  }
  else if (_menuMode == MENU_Z_RIGHT_STOP)
  {
    zStepper.setCurrentPositionToRightEndStop();
  }
  display.displayLeftEndStop(zStepper.getLeftEndStopInMM(), zStepper.isLeftEndStopSet());
  display.displayRightEndStop(zStepper.getRightEndStopInMM(), zStepper.isRightEndStopSet());
}

void GMenu::menuAttachDuringLongPress()
{
  if (_menuMode == MENU_Z_LEFT_STOP)
  {
    zStepper.disableLeftEndStop();
    display.displayLeftEndStop(zStepper.getLeftEndStopInMM(), zStepper.isLeftEndStopSet());
  }
  else if (_menuMode == MENU_Z_RIGHT_STOP)
  {
    zStepper.disableRightEndStop();
    display.displayRightEndStop(zStepper.getRightEndStopInMM(), zStepper.isRightEndStopSet());
  }
  else
  {
    zStepper.changeFeedAutoMode();
    display.displaySelectMode(zStepper.getFeedAutoMode());
  }
  delay(1000);
}

void GMenu::menuLongPressStart()
{
  display.displaySelectMode(zStepper.getFeedAutoMode());
}

