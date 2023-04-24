#ifndef __COMMON__
#define __COMMON__

#include <Arduino.h>

extern bool currentDetectionState;
extern bool currentDarkState;
extern int currentHour;
extern SemaphoreHandle_t xMutex;

#endif