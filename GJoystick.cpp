#include "GJoystick.h"

#include <Wire.h>

//https://github.com/timtro/wiinunchuck-h
#if JOYSTICK_TYPE == 1
#include <wiinunchuck.h>
#endif

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
    #if JOYSTICK_TYPE == 1
    int _nunchuk_cjoy_x = nunchuk_cjoy_x();
    #else
    int _nunchuk_cjoy_x = 140;
    #endif
    
    Serial.println(_joystickPosition);
    //Serial.println(digitalReadFast(JOYSTICK_Z_FORWARD_PIN));
#if JOYSTICK_TYPE == 1
    if (digitalReadFast(JOYSTICK_Z_FORWARD_PIN) == 4 || (_nunchuk_cjoy_x > 10 && _nunchuk_cjoy_x < 50))
#else
    if (digitalReadFast(JOYSTICK_Z_FORWARD_PIN) == 4)
#endif
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
#if JOYSTICK_TYPE == 1
    else if (digitalReadFast(JOYSTICK_Z_BACKWARD_PIN) == 4 || _nunchuk_cjoy_x > 190)
#else
    else if (digitalReadFast(JOYSTICK_Z_BACKWARD_PIN) == 4)
#endif
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
    /*else if (digitalReadFast(JOYSTICK_X_FORWARD_PIN) == 4 || nunchuk_cjoy_y() > 190)
    {
      if (_joystickPosition != 3)
      {
        _joystickPosition = 3; //X-Forward
        Serial.println("Joystick has X_FORWARD possition!");
      }
    }
    else if (digitalReadFast(JOYSTICK_X_BACKWARD_PIN) == 4 || nunchuk_cjoy_y() < 50 )
    {
      if (_joystickPosition != 4)
      {
        _joystickPosition = 4; //X-Backward
        Serial.println("Joystick has X_BACKWARD possition!");
      }
    }*/
#if JOYSTICK_TYPE == 1
    else if ((digitalReadFast(JOYSTICK_Z_FORWARD_PIN) == LOW && digitalReadFast(JOYSTICK_Z_BACKWARD_PIN) == LOW) && (_nunchuk_cjoy_x > 100 && _nunchuk_cjoy_x < 150))
#else
    else if (digitalReadFast(JOYSTICK_Z_FORWARD_PIN) == LOW && digitalReadFast(JOYSTICK_Z_BACKWARD_PIN) == LOW)
#endif
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

    /*if (nunchuk_zbutton() == HIGH && nunchuk_cbutton() == HIGH)
    {

    }*/

#if JOYSTICK_TYPE == 1
    if (digitalReadFast(JOYSTICK_BUTTON_A_PIN) == 4 || nunchuk_zbutton() == HIGH)
#else
    if (digitalReadFast(JOYSTICK_BUTTON_A_PIN) == 4)
#endif
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

#if JOYSTICK_TYPE == 1
    if (digitalReadFast(JOYSTICK_BUTTON_B_PIN) == 4 || nunchuk_cbutton() == HIGH)
#else
    if (digitalReadFast(JOYSTICK_BUTTON_B_PIN) == 4)
#endif
    {
      noInterrupts();
      zStepper.setPositionToNull();
      zHandWheelEncoder.write(zStepper.distanceInHandWheelUnits());
      interrupts();
    }

    // Daten (6 Byte) vom Nunchuk Controller auslesen
    //noInterrupts();
    #if JOYSTICK_TYPE == 1
    nunchuk_get_data();
    #endif
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
  pinModeFast(JOYSTICK_Z_FORWARD_PIN, INPUT_PULLUP);
  pinModeFast(JOYSTICK_Z_BACKWARD_PIN, INPUT_PULLUP);
  pinModeFast(JOYSTICK_BUTTON_A_PIN, INPUT_PULLUP);
  pinModeFast(JOYSTICK_BUTTON_B_PIN, INPUT_PULLUP);
  //pinModeFast(JOYSTICK_X_FORWARD_PIN, INPUT_PULLUP);
  //pinModeFast(JOYSTICK_X_BACKWARD_PIN, INPUT_PULLUP);

  //pull-up resistor
  //digitalWrite(JOYSTICK_Z_FORWARD_PIN, HIGH);
  //digitalWrite(JOYSTICK_Z_BACKWARD_PIN, HIGH);
  //digitalWrite(JOYSTICK_X_FORWARD_PIN, HIGH);
  //digitalWrite(JOYSTICK_X_BACKWARD_PIN, HIGH);

  delay(100);

#if JOYSTICK_TYPE == 1
  nunchuk_init();
  delay(100);
  nunchuk_calibrate_joy();
  delay(100);
  // Daten (6 Byte) vom Nunchuk Controller auslesen
  nunchuk_get_data();
  delay(200);
#endif

  checkCurrentJoystickPosition(millis());

  if (_joystickPosition != 0)
  {
    //2) disable stepper motors!
    zStepper.disable();
  }
}
