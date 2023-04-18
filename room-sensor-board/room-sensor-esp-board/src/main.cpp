#include <Arduino.h>
#include <WiFi.h>
#include "config.h"
#include "common.h"
#include "CommunicationTask.h"
#include "FetchDataTask.h"
#include "LedTask.h"

SemaphoreHandle_t xMutex;

void setup() {
  xMutex = xSemaphoreCreateMutex();
  setup_wifi();
  Task* fetchData = new FetchDataTask(LS_PIN,PIR_PIN);
  Task* ledTask = new LedTask(LED_PIN);
  Task* communication = new CommunicationTask();

  fetchData->init();
  ledTask->init();
  communication->init();
}

void loop() {
  // put your main code here, to run repeatedly:
}

void setup_wifi(){
  delay(10);

  Serial.println(String("Connecting to ") + ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid,password);

  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
}