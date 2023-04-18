#ifndef __COMMUNICATION_TASK__
#define __COMMUNICATION_TASK__

#include "Task.h"
#include "config.h"
#include "common.h"
#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "ArduinoJson.h"

WiFiClient espClient;
PubSubClient client(espClient); 

/* MQTT subscribing callback */
void callback(char* topic, byte* payload, unsigned int length);
void reconnect();

class CommunicationTask : public Task{
    private:
        TaskHandle_t communicationHandle;
        void tick();
        static void tickWrapper(void* _this);
    public:
        CommunicationTask();
        void init();
};

#endif