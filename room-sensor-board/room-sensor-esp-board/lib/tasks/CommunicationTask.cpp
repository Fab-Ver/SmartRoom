#include "CommunicationTask.h"

WiFiClient espClient;
PubSubClient client(espClient); 

/**
 * MQTT server address 
*/
const char* mqtt_server = "broker.mqtt-dashboard.com";

/**
 * MQTT topic
*/
const char* topic = "data";

CommunicationTask::CommunicationTask(){
    /*randomSeed(micros());
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);*/
    this->currLightState = UNDEFINED;
    this->prevLightState = UNDEFINED;
    this->currRollerBlindState = UNDETERMINED;
    this->prevRollerBlindState = UNDETERMINED;
}

void CommunicationTask::init(){
    xTaskCreatePinnedToCore(this->tickWrapper,"CommunicationTask",8192,this,1,&communicationHandle,0);
    delay(500);
}

void CommunicationTask::tickWrapper(void* _this){
	((CommunicationTask*)_this)->tick();
}

void CommunicationTask::tick(){
	for(;;){ 
        /*if(!client.connected()){
            reconnect();
        }
        client.loop();*/
        //Serial.println("Communication");
        bool detectionState;
        bool darkState;
        int hour;
        xSemaphoreTake(xMutex, portMAX_DELAY);
        detectionState = currentDetectionState;
        darkState = currentDarkState;
        hour = currentHour;
        xSemaphoreGive(xMutex);
        /* Serial.println(detectionState);
        Serial.println(darkState);
        Serial.println(hour); */
        currLightState = OFF;
        currRollerBlindState = UNDETERMINED;
        if(hour != UNDEFINED){
            if(hour >= 8 && hour < 19 && detectionState){
                currRollerBlindState = UP;
            } else if((hour >= 19 || hour <8) and !detectionState){
                currRollerBlindState = DOWN;
            }
        }
        if(detectionState && darkState){
            currLightState = ON;
        }
        if(currLightState != prevLightState){
          DynamicJsonDocument doc(1024);
          doc["light"] = currLightState == ON ? true : false;
          char* msg_json;
          serializeJson(doc,msg_json,1024);
          Serial.println(msg_json);
        }
        if(currRollerBlindState != prevRollerBlindState && currRollerBlindState != UNDETERMINED){
          DynamicJsonDocument doc(1024);
          doc["roller_blind"] = currRollerBlindState == UP ? 0 : 100;
          char* msg_json;
          serializeJson(doc,msg_json,1024);
          Serial.println(msg_json);
        }
        /*if(currentLightState != previousLightState || currentRollerBlindLevel != previousRollerBlindLevel){
            DynamicJsonDocument doc(1024);
            doc["light"] = String(currentDarkState);
            doc["rollerBlind"] = String(currentRollerBlindLevel != currentRollerBlindLevel ? currentRollerBlindLevel : NULL);
            char* msg_json;
            serializeJson(doc,msg_json,1024);
            Serial.println(msg_json);
            /*client.publish(topic,msg_json);
            previousLightState = currentLightState;
            previousRollerBlindLevel = currentRollerBlindLevel;*/
        //}
        vTaskDelay(200);
    }
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