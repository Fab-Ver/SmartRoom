#include "Pir.h"
#include <Arduino.h>

Pir::Pir(int pin){
    this->pin = pin;
    pinMode(pin,INPUT);
    Serial.print("Calibrating motion sensor");
    for(int i = 0; i < CALIBRATION_TIME_SEC; i++){
        Serial.print(".");
        delay(1000);
    }
    Serial.println("done.");
    delay(50);
}

bool Pir::isDetected(){
    return digitalRead(pin);
}