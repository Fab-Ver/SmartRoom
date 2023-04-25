#ifndef __FETCH_DATA_TASK__
#define __FETCH_DATA_TASK__

#include "Task.h"
#include "LightSensor.h"
#include "Pir.h"
#include "time.h"
#include <Arduino.h>
#include "commons.h"
#include "config.h"

int getCurrentHour();

class FetchDataTask : public Task{
    private:
        MotionSensor* pir;
        PhotoResistor* lightSensor;
        TaskHandle_t fetchDataHandle;
        void tick();
        static void tickWrapper(void* _this);
    public:
        FetchDataTask(int lsPin, int pirPIN);
        void init();
};

#endif