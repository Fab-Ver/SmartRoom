#include "RollerBlind.h"

RollerBlind::RollerBlind(int pin){
    this->pin = pin;
}

void RollerBlind::on(){
    motor.attach(pin);
}

void RollerBlind::off(){
    motor.detach();
}

void RollerBlind::setPosition(int angle){
    if(angle > 180){
        angle = 180;
    } else if(angle < 0){
        angle = 0;
    }
    float coeff = ((float)(MAX_PULSE_WIDTH - MIN_PULSE_WIDTH))/180.0;
    motor.write(MIN_PULSE_WIDTH + angle * coeff);
}

