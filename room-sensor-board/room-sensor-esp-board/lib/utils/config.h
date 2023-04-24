#ifndef __CONFIG__
#define __CONFIG__

#define LED_PIN 18
#define PIR_PIN 46
#define LS_PIN 3
#define UNDEFINED_HOUR -1

typedef enum LightState {ON,OFF,UNDEFINED} LightState;
typedef enum RollerBlindState {UP,DOWN,UNDETERMINED} RollerBlindState;

#endif