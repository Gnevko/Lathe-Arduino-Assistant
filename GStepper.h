#ifndef GNEVKOSTEPPER_H
#define GNEVKOSTEPPER_H

#include "Config.h"

#include <stdlib.h>
#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#include <wiring.h>
#endif

#define MANUALLY_FEED_MODE 0
#define AUTO_FEED_MODE 1
#define AUTO_FEED_MODE_ASYNC 0
#define AUTO_FEED_MODE_THREAD 1
#define DIRECTION_LEFT 1
#define DIRECTION_RIGHT -1


class GStepper
{
  public:
    GStepper();
    virtual void    disable();
    virtual void    enable();
    void    moveTo(long absolute);
    //void    setSpeed(double speed, char dir);
    void    setSpeed(float speed);
    long    moveToWithHandWheel(long handWheelPosition);
    void    run();
    void    runSpeed();
    long distanceToGo();
    volatile long getCurrentPosition();
    float distanceInMM();
    long distanceInHandWheelUnits();
    void resetPosition();
    void setPositionToNull();

    void setLeftEndStopInSteps(long leftStop);
    void setRightEndStopInSteps(long rightStop);
    long getLeftEndStopInSteps();
    float getLeftEndStopInMM();
    long getRightEndStopInSteps();
    float getRightEndStopInMM();
    void setEndStopsToZero();
    void setCurrentPositionToLeftEndStop();
    void setCurrentPositionToRightEndStop();

    void increaseAutoSpeed();
    void decreaseAutoSpeed();
    void quickAutoSpeed();
    void backToNormalAutoSpeed();
    void increaseAutoSyncSpeed();
    void decreaseAutoSyncSpeed();
    float getAutoSpeed();
    float getAutoSyncSpeed();
    byte getFeedMode();
    void setFeedMode(byte feedMode);
    byte getFeedAutoMode();
    void changeFeedAutoMode();
    void setFeedAutoMode(byte feedAutoMode);
    void setDirection(char direction);
    char getDirection();

    //HandWheel
    void calculateSpeedForHandWheel(long newHandWheelPosition);
    float getHandWheelSpeed();
    void setHandWheelFactor(float handWheelFactor);

    volatile long _delta; //microseconds
  protected:
    void  makeStep();
    boolean isEndStopsSet();
  private:
    volatile unsigned long _lastStepTime; //microseconds
    volatile unsigned long _nowTime; //microseconds
    volatile unsigned long _deltaTimeForNextStep;

    volatile long           _currentPos;    // Steps
    volatile long           _targetPos;     // Steps

    //for handwheel
    long _oldHandWheelPosition;
    float _handWheelSpeed;
    float _handwheelFactor;

    //for speed mode
    volatile float _speed;

    //for stops
    volatile long _leftEndStop; // Steps
    volatile long _rightEndStop; // Steps

    // a variable for the selected auto feed sync speed: mm / one revolution of spindle: 0.08; 0.12; 0.16;
    int _autoSyncSpeedCount = 0;
    //ATTENTION! The values are multiplayed by 100 for better precision and quick calclations
    const float _zAutoFeedSyncSpeeds[NUM_AUTO_SPEEDS_SYNC] = {0.08, 0.12, 0.16, 0.30, 0.40, 0.50, 0.60, 0.70, 0.75, 0.80, 1.00, 1.25, 1.50, 1.75, 2.00, 2.50, 3.00};

    //a variable for the selected auto feed speed: mm / second
    int _autoSpeedCount = 0;
    int _oldAutoSpeedCount = 0;
    boolean _isQuickAutoSpeed = false;
    //ATTENTION! The values are multiplayed by 100 for better precision and quick calclations
    const float _zAutoFeedSpeeds[NUM_AUTO_SPEEDS] = {0.05, 0.10, 2.00, 6.00};

    // a variable for mini lathe feed mode: 0 - manually feed; 1 - auto feed;
    volatile byte _feedMode;
    //0 - async, 1 - thread
    volatile byte _feedAutoMode;
    // a variable for diraction (+ or -): 1 - left; -1 - right;
    volatile char _dir;
};


#endif //GNEVKOSTEPPER_H
