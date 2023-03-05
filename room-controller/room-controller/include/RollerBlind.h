#ifndef __ROLLER_BLIND__
#define __ROLLER_BLIND__

#include "ServoMotor.h"
#include <Arduino.h>
#include "ServoTimer2.h"

class RollerBlind : public ServoMotor{
    private:
        ServoTimer2 motor;
        int pin;
    public:
        RollerBlind(int pin);
        void on();
        void off();
        void setPosition(int angle);
};
    
#endif