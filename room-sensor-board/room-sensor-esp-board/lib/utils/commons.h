#ifndef __COMMONS__
#define __COMMONS__

#include <Arduino.h>

extern bool currentDetectionState;
extern bool currentDarkState;
extern int currentHour;
extern SemaphoreHandle_t xMutex;

#endif