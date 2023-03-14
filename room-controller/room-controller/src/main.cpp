#include <Arduino.h>
#include "config.h"
#include "ModuleBT.h"
#include "SerialCommunicator.h"
#include "Led.h"
#include "RollerBlind.h"
#include "ArduinoJson.h"

ModuleBT* bt;
SerialCommunicator* serial = new SerialCommunicator();
Light* led;
RollerBlind* rb;

void setup() {
  bt = new ModuleBT(RX_PIN, TX_PIN);
  bt->init();
  serial->init();
  led = new Led(LED_PIN);
  rb = new RollerBlind(SERVO_PIN);
}

void loop() {
  if(serial->isMsgAvailable()){
    Msg* msg = serial->receiveMsg();
    bt->sendMsg(Msg(msg->getContent()));

    DynamicJsonDocument doc(1024);
    deserializeJson(doc,msg->getContent());
    String led_state = doc["led_state"];
    int rb_angle = doc["rb_angle"];

    if(led_state == "ON"){
      led->switchOn();
    } else {
      led->switchOff();
    }

    rb->on();
    rb->setPosition(rb_angle);
    rb->off();

    delete msg;
  }

  if(bt->isMsgAvailable()){
    Msg* msg = bt->receiveMsg();
    serial->sendMsg(Msg(msg->getContent()));
    delete msg;
  }
}