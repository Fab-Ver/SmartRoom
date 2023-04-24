#ifndef __LED_TASK__
#define __LED_TASK__

#include "Task.h"
#include "Led.h"
#include <Arduino.h>
#include "commons.h"
#include "config.h"

class LedTask : public Task {
    private:
        Light* led;
        TaskHandle_t LedHandle;
        LightState currState;
        LightState previousState;
        void tick();
        static void tickWrapper(void* _this);
    public:
        LedTask(int ledPin);
        void init();
};

#endif