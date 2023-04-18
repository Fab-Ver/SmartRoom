#include "CommunicationTask.h"

bool previousLightState = NULL;
int previousRollerBlindLevel = NULL;

CommunicationTask::CommunicationTask(){
    randomSeed(micros());
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
}

void CommunicationTask::init(){
    xTaskCreatePinnedToCore(this->tickWrapper,"CommunicationTask",1000,this,1,&communicationHandle,0);
    delay(500);
}

void CommunicationTask::tickWrapper(void* _this){
	((CommunicationTask*)_this)->tick();
}

void CommunicationTask::tick(){
	for(;;){ 
        if(!client.connected()){
            reconnect();
        }
        client.loop();
        bool detectionState;
        bool darkState;
        int hour;
        xSemaphoreTake(xMutex, portMAX_DELAY);
        detectionState = currentDetectionState;
        darkState = currentDarkState;
        hour = currentHour;
        xSemaphoreGive(xMutex);
        bool currentLightState = false;
        int currentRollerBlindLevel = previousRollerBlindLevel;
        if(hour != NULL){
            if(hour >= 8 && hour < 19 && detectionState){
                currentRollerBlindLevel = 0;
            } else if((hour >= 19 || hour <8) and !detectionState){
                currentDetectionState = 100;
            }
        }
        if(detectionState && darkState){
            currentLightState = true;
        }
        if(currentLightState != previousLightState || currentRollerBlindLevel != previousRollerBlindLevel){
            DynamicJsonDocument doc(1024);
            doc['light'] = String(currentDarkState);
            doc['rollerBlind'] = String(currentRollerBlindLevel != currentRollerBlindLevel ? currentRollerBlindLevel : NULL);
            char* msg_json;
            serializeJson(doc,msg_json);
            client.publish(topic,msg_json);
            previousLightState = currentLightState;
            previousRollerBlindLevel = currentRollerBlindLevel;
        }  
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