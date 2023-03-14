#include <Arduino.h>
#include "config.h"
#include "ModuleBT.h"
#include "SerialCommunicator.h"
#include "Led.h"
#include "RollerBlind.h"

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
    /*JSON e controlli*/
    delete msg;
  }

  if(bt->isMsgAvailable()){
    Msg* msg = bt->receiveMsg();
    serial->sendMsg(Msg(msg->getContent()));
    delete msg;
  }
}