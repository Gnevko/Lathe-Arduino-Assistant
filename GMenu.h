#ifndef GMENU_H
#define GMENU_H

#include "Config.h"
#include "GHandWheel.h"
#include "GDisplay.h"
#include "GStepper.h"

#include <stdlib.h>
#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#include <wiring.h>
#endif

extern GStepper zStepper;
extern GHandWheel zHandWheel;
extern GDisplay display;

class GMenu
{
  public:
    GMenu();
    void checkMenu(unsigned long timeNow, int menuCounter);
    void menuClick();
    void menuDoubleClick();
    void menuAttachDuringLongPress();
    void menuLongPressStart();
    
  protected:

  private:
    int _menuMode;
    unsigned long _lastMenuChanges;
    int _menuOldCounter;
};

#endif //GMENU_H
