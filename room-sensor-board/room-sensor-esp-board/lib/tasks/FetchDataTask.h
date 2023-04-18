#ifndef __FETCH_DATA_TASK__
#define __FETCH_DATA_TASK__

#define TASK_PERIOD 200

#include "Task.h"
#include "LightSensor.h"
#include "Pir.h"
#include "time.h"
#include <Arduino.h>
#include "common.h"

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;
bool currentDetectionState = false;
bool currentDarkState = false;
int currentHour = NULL;

class FetchDataTask : public Task{
    private:
        PhotoResistor* lightSensor;
        MotionSensor* pir;
        TaskHandle_t fetchDataHandle;
        void tick();
        static void tickWrapper(void* _this);
    public:
        FetchDataTask(int lsPin, int pirPIN);
        void init();
};

#endif