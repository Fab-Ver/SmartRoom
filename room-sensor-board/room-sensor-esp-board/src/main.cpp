#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "config.h"
#include "Led.h"
#include "LightSensor.h"
#include "Pir.h"
#include "ArduinoJson.h"

Light* led;
PhotoResistor* ls;
MotionSensor* pir;

TaskHandle_t CommunicationHandle;
TaskHandle_t LedHandle;

void CommunicationTask(void* parameter);
void LedTask(void* parameter);
void setup_wifi();
void reconnect();

/* MQTT subscribing callback */
void callback(char* topic, byte* payload, unsigned int length);

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0; 

void setup() {
  led = new Led(LED_PIN);
  ls = new LightSensor(LS_PIN);
  pir = new Pir(PIR_PIN);
  setup_wifi();
  randomSeed(micros());
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
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
    if(!client.connected()){
      reconnect();
    }
    client.loop();
    unsigned long now = millis();
    if(now - lastMsg >= MSG_PERIOD){
      lastMsg = now;
      DynamicJsonDocument doc(1024);
      doc['detected'] = String(pir->isDetected());
      doc['dark'] = String(ls->isDark());
      char* msg_json;
      serializeJson(doc,msg_json);
      client.publish(topic,msg_json);
    }
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

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.println(String("Message arrived on [") + topic + "] len: " + length );
}

void reconnect(){
  while(!client.connected()){
    Serial.print("Attempting MQTT connection...");

    /*Create a random client ID*/
    String clientID = String("room-client-") + String(random(0xffff), HEX);

    /*Attempt to connect*/
    if(client.connect(clientID.c_str())){
      Serial.println("connected");
      client.subscribe(topic);
    } else {
      Serial.print("failed rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds.");
      delay(5000);
    }
  }
}