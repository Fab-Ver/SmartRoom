#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "config.h"
#include "Led.h"
#include "LightSensor.h"
#include "Pir.h"

Light* led;
PhotoResistor* ls;
MotionSensor* pir;

TaskHandle_t CommunicationHandle;
TaskHandle_t LedHandle;

void CommunicationTask(void* parameter);
void LedTask(void* parameter);

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  led = new Led(LED_PIN);
  ls = new LightSensor(LS_PIN);
  pir = new Pir(PIR_PIN);
  xTaskCreatePinnedToCore(CommunicationTask,"CommunicationTask",1000,NULL,1,&CommunicationHandle,0);
  delay(500);
  xTaskCreatePinnedToCore(LedTask,"LedTask",1000,NULL,1,&LedHandle,1);
  delay(500);
}

void loop() {
  // put your main code here, to run repeatedly:
}

void CommunicationTask(void* paramater){
  for(;;){
    
  }
}

void LedTask(void* paramater){
  for(;;){
    if(pir->isDetected()){
      led->switchOn();
    } else {
      led->switchOff();
    } 
  }
}