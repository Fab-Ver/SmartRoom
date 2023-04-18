#include "FetchDataTask.h"

unsigned long lastDetection = 0;

FetchDataTask::FetchDataTask(int lsPin, int pirPin){
    this->lightSensor = new LightSensor(lsPin);
    this->pir = new Pir(pirPin);
}

void FetchDataTask::init(){
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    xTaskCreatePinnedToCore(this->tickWrapper, "FetchDataTask", 1000, this, 3, &fetchDataHandle, 0);
    delay(500);
}

void FetchDataTask::tickWrapper(void* _this){
	((FetchDataTask*)_this)->tick();
}

void FetchDataTask::tick(){
	for(;;){ 
        unsigned long now = millis();
        if(now - lastDetection >= TASK_PERIOD){
            lastDetection = now;
            int hour = getCurrentHour();
            bool dark = lightSensor->isDark();
            bool detected = pir->isDetected();
            xSemaphoreTake(xMutex, portMAX_DELAY);
            currentHour = hour;
            currentDarkState = dark;
            currentDetectionState = detected;
            xSemaphoreGive(xMutex);
        }
    }
}

int getCurrentHour(){
    struct tm timeInfo;
    if(!getLocalTime(&timeInfo)){
        Serial.println("Failed to obtain time");
        return NULL;
    }
    return timeInfo.tm_hour;
}