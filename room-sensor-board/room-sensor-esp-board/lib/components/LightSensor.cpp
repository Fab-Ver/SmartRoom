#include "LightSensor.h"
#include <Arduino.h>

LightSensor::LightSensor(int pin){
    this->pin = pin;
}

bool LightSensor::isDark(){
    return analogRead(pin) < THRESHOLD;
}