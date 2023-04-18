#ifndef __PIR__
#define __PIR__

#include "MotionSensor.h"
#define CALIBRATION_TIME_SEC 20

class Pir : public MotionSensor{
    private: 
        int pin;
    public:
        Pir(int pin);
        bool isDetected();
};

#endif