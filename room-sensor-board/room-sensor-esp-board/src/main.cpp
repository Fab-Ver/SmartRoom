#include <Arduino.h>
#include <WiFi.h>
#include "config.h"
#include "CommunicationTask.h"
#include "FetchDataTask.h"
#include "LedTask.h"
#include "commons.h"

/**
 * WIFI network info
*/
const char* ssid = "iPhone di Fabio";
const char* password = "fabio0104";

SemaphoreHandle_t xMutex;
void setup_wifi();

void setup() {
  Serial.begin(115200);
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
  delay(1000000);
}

void setup_wifi(){
  delay(10);

  Serial.print(String("Connecting to ") + ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid,password);

  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }

  Serial.println("WiFi connected");
}