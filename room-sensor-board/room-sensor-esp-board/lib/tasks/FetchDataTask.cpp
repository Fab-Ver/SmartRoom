#include "FetchDataTask.h"

unsigned long lastDetection = 0;
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;
bool currentDetectionState = false;
bool currentDarkState = false;
int currentHour = UNDEFINED_HOUR;

FetchDataTask::FetchDataTask(int lsPin, int pirPin){
    this->lightSensor = new LightSensor(lsPin);
    this->pir = new Pir(pirPin);
}

void FetchDataTask::init(){
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    xTaskCreatePinnedToCore(this->tickWrapper, "FetchDataTask", 8192, this, 2, &fetchDataHandle, 0);
    delay(500);
}

void FetchDataTask::tickWrapper(void* _this){
	((FetchDataTask*)_this)->tick();
}

void FetchDataTask::tick(){
	for(;;){ 
        unsigned long now = millis();
        if(now - lastDetection >= TASK_PERIOD){
            /* Serial.println("Fetch"); */
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
        vTaskDelay(200);
    }
}

int getCurrentHour(){
    struct tm timeInfo;
    if(!getLocalTime(&timeInfo)){
        Serial.println("Failed to obtain time");
        return UNDEFINED_HOUR;
    }
    return timeInfo.tm_hour;
}