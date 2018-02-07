#include "GJoystick.h"

#include <Wire.h>
#include <wiinunchuck.h>

GJoystick::GJoystick()
{
  _joystickPosition = -1;
  _lastJoysticChanges = -60;
}

/**

*/
void GJoystick::checkCurrentJoystickPosition(unsigned long timeNow) {

  if (timeNow - _lastJoysticChanges > JOYSTICK_REACTION_TIME)
  {
    int _nunchuk_cjoy_x = nunchuk_cjoy_x();
    //Serial.println(_joystickPosition);
    //Serial.println(_nunchuk_cjoy_x);
    if (digitalReadFast(JOYSTICK_Z_FORWARD_PIN) == LOW || (_nunchuk_cjoy_x > 10 && _nunchuk_cjoy_x < 50))
    {
      if (_joystickPosition != 1)
      {
        noInterrupts();
        _joystickPosition = 1; //Z-Left/Forward
        zStepper.resetPosition();
        zStepper.setFeedMode(AUTO_FEED_MODE); //auto feed
        zStepper.setDirection(DIRECTION_LEFT);
        if (zStepper.getFeedAutoMode() == AUTO_FEED_MODE_ASYNC) //async
        {
          zStepper.setSpeed(zStepper.getAutoSpeed());
        }
        else
        {
          spindle.reset();
        }
        interrupts();
        display.displayMode(zStepper.getFeedMode(), zStepper.getFeedAutoMode(), zStepper.getDirection());
      }
    }
    else if (digitalReadFast(JOYSTICK_Z_BACKWARD_PIN) == LOW || _nunchuk_cjoy_x > 190)
    {
      if (_joystickPosition != 2)
      {
        noInterrupts();
        zStepper.resetPosition();
        _joystickPosition = 2; //Z-Right/Backward
        zStepper.setFeedMode(AUTO_FEED_MODE); //auto feed
        zStepper.setDirection(DIRECTION_RIGHT);
        if (zStepper.getFeedAutoMode() == AUTO_FEED_MODE_ASYNC) //async
        {
          zStepper.setSpeed(zStepper.getAutoSpeed());
        }
        else
        {
          spindle.reset();
        }
        interrupts();
        display.displayMode(zStepper.getFeedMode(), zStepper.getFeedAutoMode(), zStepper.getDirection());
      }
    }
    else if (digitalReadFast(JOYSTICK_X_FORWARD_PIN) == LOW || nunchuk_cjoy_y() > 190)
    {
      if (_joystickPosition != 3)
      {
        _joystickPosition = 3; //X-Forward
        Serial.println("Joystick has X_FORWARD possition!");
      }
    }
    else if (digitalReadFast(JOYSTICK_X_BACKWARD_PIN) == LOW || nunchuk_cjoy_y() < 50 )
    {
      if (_joystickPosition != 4)
      {
        _joystickPosition = 4; //X-Backward
        Serial.println("Joystick has X_BACKWARD possition!");
      }
    }
    else if (_nunchuk_cjoy_x > 100 && _nunchuk_cjoy_x < 150)
    {
      if (_joystickPosition != 0)
      {
        _joystickPosition = 0; //neutral
        noInterrupts();
        zStepper.setFeedMode(MANUALLY_FEED_MODE);// manually feed
        zStepper.resetPosition();
        zHandWheelEncoder.write(zStepper.distanceInHandWheelUnits());
        interrupts();
        //enable stepper motors in neutral position
        zStepper.enable();
        display.displayMode(zStepper.getFeedMode(), zStepper.getFeedAutoMode(), zStepper.getDirection());
        display.displayHandWheelFactor(zHandWheel.getSpeed());
        display.displayAutoSpeed(zStepper.getAutoSpeed());
        display.displaySyncSpeed(zStepper.getSyncSpeed());
        display.displayLeftEndStop(zStepper.getLeftEndStopInMM(), zStepper.isLeftEndStopSet());
        display.displayRightEndStop(zStepper.getRightEndStopInMM(), zStepper.isRightEndStopSet());
      }
    }

    if (nunchuk_zbutton() == HIGH && nunchuk_cbutton() == HIGH)
    {

    }

    if (nunchuk_zbutton() == HIGH)
    {
      if (zStepper.getFeedMode() == AUTO_FEED_MODE && zStepper.getFeedAutoMode() == AUTO_FEED_MODE_ASYNC)
      {
        if (!zStepper.getIsQuickAutoSpeed())
        {
          //noInterrupts();
          //Serial.println("nunchuk_zbutton() == HIGH");
          //zStepper.resetPosition();
          zStepper.setQuickAutoSpeed();
          zStepper.setSpeed(zStepper.getAutoSpeed());
          display.displayAutoSpeed(zStepper.getAutoSpeed());
          //interrupts();
        }
      }
      else if (zStepper.getFeedMode() == AUTO_FEED_MODE && zStepper.getFeedAutoMode() == AUTO_FEED_MODE_SYNC)
      {
        if (!zStepper.getIsQuickSyncSpeed())
        {
          zStepper.setQuickSyncSpeed();
          display.displaySyncSpeed(zStepper.getSyncSpeed());
        }
      }
    }
    else
    {
      if (zStepper.getFeedMode() == AUTO_FEED_MODE && zStepper.getFeedAutoMode() == AUTO_FEED_MODE_ASYNC)
      {
        if (zStepper.getIsQuickAutoSpeed())
        {
          //noInterrupts();
          //Serial.println("nunchuk_zbutton() == LOW");
          //zStepper.resetPosition();
          zStepper.backToNormalAutoSpeed();
          zStepper.setSpeed(zStepper.getAutoSpeed());
          display.displayAutoSpeed(zStepper.getAutoSpeed());
          //interrupts();
        }
      }
      else if (zStepper.getFeedMode() == AUTO_FEED_MODE && zStepper.getFeedAutoMode() == AUTO_FEED_MODE_SYNC)
      {
        if (zStepper.getIsQuickSyncSpeed())
        {
          zStepper.backToNormalSyncSpeed();
          display.displaySyncSpeed(zStepper.getSyncSpeed());
        }
      }
    }

    if (nunchuk_cbutton() == HIGH)
    {
      noInterrupts();
      zStepper.setPositionToNull();
      zHandWheelEncoder.write(zStepper.distanceInHandWheelUnits());
      interrupts();
    }

    // Daten (6 Byte) vom Nunchuk Controller auslesen
    //noInterrupts();
    nunchuk_get_data();
    //interrupts();
    _lastJoysticChanges = timeNow;
  }
}

/**
   init Joystick
*/
void GJoystick::initJoystick()
{
  //Serial.println("initJoystick");
  // input
  pinMode(JOYSTICK_Z_FORWARD_PIN, INPUT);
  pinMode(JOYSTICK_Z_BACKWARD_PIN, INPUT);
  pinMode(JOYSTICK_X_FORWARD_PIN, INPUT);
  pinMode(JOYSTICK_X_BACKWARD_PIN, INPUT);

  //pull-up resistor
  digitalWrite(JOYSTICK_Z_FORWARD_PIN, HIGH);
  digitalWrite(JOYSTICK_Z_BACKWARD_PIN, HIGH);
  digitalWrite(JOYSTICK_X_FORWARD_PIN, HIGH);
  digitalWrite(JOYSTICK_X_BACKWARD_PIN, HIGH);

  delay(100);

  nunchuk_init();
  delay(100);
  nunchuk_calibrate_joy();
  delay(100);
  // Daten (6 Byte) vom Nunchuk Controller auslesen
  nunchuk_get_data();
  delay(200);

  checkCurrentJoystickPosition(millis());

  if (_joystickPosition != 0)
  {
    //2) disable stepper motors!
    zStepper.disable();
  }
}
