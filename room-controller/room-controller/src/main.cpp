#include <Arduino.h>
#include "config.h"
#include "BluetoothCommunicator.h"
#include "SerialCommunicator.h"
#include "Led.h"
#include "RollerBlind.h"
#include "ArduinoJson.h"

BluetoothCommunicator* bluetooth;
SerialCommunicator* serial;
Light* led;
RollerBlind* rollerBlind;

LightState prevLightState = UNDEFINED;
int prevRollerBlindState = UNKNOWN;

void modifyState(String msg_json);

void setup() {
	serial = new SerialCommunicator();
	bluetooth = new BluetoothCommunicator(RX_PIN, TX_PIN);
	serial->init();
	bluetooth->init();
	led = new Led(LED_PIN);
	rollerBlind = new RollerBlind(SERVO_PIN);
}

void loop() {
	if(serial->isMsgAvailable()){
    	Msg* msg = serial->receiveMsg();
    	modifyState(msg->getContent());
    	bluetooth->sendMsg(Msg(msg->getContent()));
    	delete msg;
  	}
  	if(bluetooth->isMsgAvailable()){
    	Msg* msg = bluetooth->receiveMsg();
    	modifyState(msg->getContent());
    	serial->sendMsg(Msg(msg->getContent()));
    	delete msg;
  	}
}

void modifyState(String msg_json){
	StaticJsonDocument<200> doc;
	DeserializationError error = deserializeJson(doc, msg_json);
	if(error){
		Serial.print(F("deserializeJson() failed: "));
    	Serial.println(error.f_str());
    	return;
	}
	JsonObject json = doc.as<JsonObject>();
	if(json.containsKey("light")){
		bool light = doc["light"];
		LightState currLightState = light ? ON : OFF;
		if(currLightState != prevLightState){
			if(currLightState == ON){
				led->switchOn();
			} else {
				led->switchOff();
			}
			prevLightState = currLightState;
		}
	}
	if(json.containsKey("roller_blind")){
		int currRollerBlindState = doc["roller_blind"];
		if(currRollerBlindState != prevRollerBlindState){
			int angle = map(currRollerBlindState, 0, 100, 0, 180);
			rollerBlind->setPosition(angle);
			prevRollerBlindState = currRollerBlindState;
		}
	}
}

