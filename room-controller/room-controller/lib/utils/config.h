#ifndef __CONFIG__
#define __CONFIG__

/* RX is pin 2 => to be connected to TXD of BT module*/
#define RX_PIN 12 
/* TX is pin3 => to be connected to RXD of BT module*/
#define TX_PIN 13
#define LED_PIN 7
#define SERVO_PIN 4
#define UNKNOWN -1

typedef enum LightState {ON, OFF, UNDEFINED} LightState;

#endif