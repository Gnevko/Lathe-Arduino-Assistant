//https://github.com/PaulStoffregen/Encoder
// This optional setting causes Encoder to use more optimized code,
// It must be defined before Encoder.h is included.
#include <Encoder.h>

//https://github.com/NicksonYap/digitalWriteFast
#include <digitalWriteFast.h>

//http://www.mathertel.de/Arduino/OneButtonLibrary.aspx
#include <OneButton.h>

//https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library
#include <LiquidCrystal_I2C.h>

//https://github.com/PaulStoffregen/TimerThree
#include <TimerThree.h>

/*********************************************************
   Spidle Encoder
 *********************************************************/
#define SPINDLE_A 2

/*********************************************************
   ZHandwheel
 *********************************************************/
#define Z_HANDWHEEL_A 19
#define Z_HANDWHEEL_B 18

/*********************************************************
   Menu Encoder
 *********************************************************/
#define MENU_HANDWHEELFACTOR 1
#define MENU_Z_AUTOFEEDSPEED 2
#define MENU_Z_AUTOFEEDSPEEDSYNC 3
#define MENU_Z_LEFT_STOP 4
#define MENU_Z_RIGHT_STOP 5

#define MENU_A 4
#define MENU_B 5
#define MENU_BUTTON 6
#define MENU_REACTION_TIME 10

/*********************************************************
   Joystick
 *********************************************************/
#define JOYSTICK_Z_FORWARD_PIN  33
#define JOYSTICK_Z_BACKWARD_PIN 35
#define JOYSTICK_X_FORWARD_PIN  37
#define JOYSTICK_X_BACKWARD_PIN 39

#define JOYSTICK_REACTION_TIME 100

/*********************************************************
   Z Axe Stepper
 *********************************************************/
#define Z_STEP_PIN 23
#define Z_DIR_PIN 27
#define Z_ENABLE_PIN 31

//#define MM_PER_Z_STEP 0.000625 //2mm / 3200 steps;
//#define MM_PER_Z_STEP_HANDWHEEL 0.0025 //(2mm / 3200 steps) * 4 'ticks';

//1600 micro steps
//#define MM_PER_Z_STEP 0.00125 //2mm / 1600 steps;
//#define MM_PER_Z_STEP_HANDWHEEL 0.005 //(2mm / 1600 steps) * 4 'ticks';

//800 steps
#define MM_PER_Z_STEP 0.0025 //2mm / 800 steps;
#define MM_PER_Z_STEP_HANDWHEEL 0.01 //(2mm / 800 steps) * 4 'ticks';

//200 steps
//#define MM_PER_Z_STEP 0.01 //2mm / 200 steps;
//#define MM_PER_Z_STEP_HANDWHEEL 0.04 //(2mm / 200 steps) * 4 'ticks';
/*********************************************************
   Speeds
 *********************************************************/
#define NUM_AUTO_SPEEDS 4
#define QUICK_AUTO_SPEED_POSITION 3
#define NUM_SYNC_SPEEDS 17
#define QUICK_SYNC_SPEED_POSITION 14
