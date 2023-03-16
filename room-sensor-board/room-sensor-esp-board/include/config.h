#define LED_PIN 1
#define PIR_PIN 2
#define LS_PIN 3
#define MSG_PERIOD 200
#define MSG_BUFFER_SIZE 50

/**
 * WIFI network info
*/
const char* ssid = "nome";
const char* password = "password";

/**
 * MQTT server address 
*/
const char* mqtt_server = "broker.mqtt-dashboard.com";

/**
 * MQTT topic
*/
const char* topic = "data";