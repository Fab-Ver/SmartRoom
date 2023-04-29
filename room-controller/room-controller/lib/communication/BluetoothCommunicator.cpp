#include "BluetoothCommunicator.h"

BluetoothCommunicator::BluetoothCommunicator(int rxPin, int txPin){
    this->channel = new SoftwareSerial(rxPin,txPin);
}

void BluetoothCommunicator::init(){
    content.reserve(256);
    channel->begin(9600);
    channel->print("AT+NAMEroom_controller");
    content = "";
    currentMsg = NULL;
    availableMsg = false;
}

bool BluetoothCommunicator::isMsgAvailable(){
    while (channel->available()){
        char ch = (char) channel->read();
        if(ch == '\n'){
            currentMsg = new Msg(content);
            availableMsg = true;
            return true;
        } else {
            content += ch;
        }
    }
    return false;
}

Msg* BluetoothCommunicator::receiveMsg(){
    if(availableMsg){
        Msg* msg = currentMsg;
        availableMsg = false;
        currentMsg = NULL;
        content = "";
        return msg;
    }
    return NULL;
}

void BluetoothCommunicator::sendMsg(Msg msg){
    channel->println(msg.getContent());
}

