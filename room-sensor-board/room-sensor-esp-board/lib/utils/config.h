#ifndef __CONFIG__
#define __CONFIG__

#define LED_PIN 18
#define PIR_PIN 46
#define LS_PIN 3
#define UNDEFINED_HOUR -1
#define TASK_PERIOD 200

/*
    Time config constants
*/
#define GMT_OFFSET_SEC 3600
#define DAYLIGHT_OFFSET_SEC 3600

typedef enum LightState {ON,OFF,UNDEFINED} LightState;
typedef enum RollerBlindState {UP,DOWN,UNDETERMINED} RollerBlindState;

#endif