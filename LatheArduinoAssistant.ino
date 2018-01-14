
#include "Config.h"
#include "GDisplay.h"
#include "GSpindle.h"
#include "GStepper.h"
#include "GHandWheel.h"
#include "GJoystick.h"
#include "GMenu.h"

/*********************************************************
   Common variables
 *********************************************************/
unsigned long timeNow = 0;
unsigned long timeOld = 0;
int temp;

LiquidCrystal_I2C lcd(0x3F, 20, 4);
GStepper zStepper;
GDisplay display(&lcd);
GSpindle spindle(SPINDLE_A);
Encoder zHandWheelEncoder(Z_HANDWHEEL_B, Z_HANDWHEEL_A);
GHandWheel zHandWheel;
Encoder menuEncoder(MENU_A, MENU_B);
GMenu menu;
GJoystick joystick;
OneButton menuEncoderButton(MENU_BUTTON, true);

void setup() {
  Serial.begin(9600);
  // initialize the LCD
  lcd.begin();
  lcd.print(F("Init ..."));

  joystick.initJoystick();

  zStepper.setHandWheelFactor(zHandWheel.getSpeed());

  attachInterrupt(0, callbackSpindle, RISING); //PIN2

  //Timer3.initialize(20);
  //Timer3.attachInterruptFunction(callbackTimer, 20);  // attaches callback() as a timer overflow interrupt

  menuEncoderButton.attachClick(callbackMenuClick);
  menuEncoderButton.attachDoubleClick(callbackMenuDoubleClick);
  menuEncoderButton.attachLongPressStart(callbackMenuLongPressStart);
  menuEncoderButton.attachDuringLongPress(callbackMenuAttachDuringLongPress);

  lcd.print(F("OK"));
  delay(500);
  lcd.clear();
}

volatile long oldPosition = 0;

void callbackSpindle()
{ 
  //spindle.callbackSpindleUpdate();
  //Serial.println(zStepper.getCurrentPosition() - oldPosition);
  oldPosition = zStepper.getCurrentPosition();
  
  if (zStepper.getFeedMode() == AUTO_FEED_MODE && zStepper.getFeedAutoMode() == AUTO_FEED_MODE_THREAD && spindle.isReady() )
  {
    //Serial.println(spindle.calculateMMPerSpindleRevolutionPerSecond(zStepper.getAutoSyncSpeed()));
    zStepper.setSpeed(spindle.calculateMMPerSpindleRevolutionPerSecond(zStepper.getAutoSyncSpeed()));
  }
}

void callbackTimer()
{
  if (zStepper.getFeedMode() == MANUALLY_FEED_MODE) //manually feed
  {
    zStepper.run();
  }
  else
  {
    zStepper.runSpeed();
  }
}

void callbackMenuClick()
{
  menu.menuClick();
}

void callbackMenuDoubleClick()
{
  menu.menuDoubleClick();
}

void callbackMenuAttachDuringLongPress()
{
  menu.menuAttachDuringLongPress();
}

void callbackMenuLongPressStart()
{
  menu.menuLongPressStart();
}

void loop() {

  timeNow = millis();

  joystick.checkCurrentJoystickPosition(timeNow);
  menuEncoderButton.tick();
  menu.checkMenu(timeNow, menuEncoder.read());

  if (zStepper.getFeedMode() == MANUALLY_FEED_MODE) //manually feed
  {
    zHandWheel.handleUpdate(timeNow, zHandWheelEncoder.read());
  }

  //Serial.println(zStepper._delta);
  if (timeNow - timeOld > 200)
  {
    //lcd.setCursor(1, 3);
    //lcd.print(temp++);
    //display.displayDistance(temp++);
    display.displayDistance(zStepper.distanceInMM());
    display.displaySpindleSpeed(spindle.calculateSpindleRevolutionsPerMinute());
    timeOld = timeNow;
  }

  
}
