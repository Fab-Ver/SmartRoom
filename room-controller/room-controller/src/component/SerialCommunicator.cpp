#include "SerialCommunicator.h"
#include "Arduino.h"

void SerialCommunicator::init(){
    Serial.begin(9600);
    content.reserve(256);
    content = "";
    currentMsg = NULL;
    availableMsg = false;
}

void SerialCommunicator::sendMsg(Msg msg){
    Serial.println(msg.getContent());
}

bool SerialCommunicator::isMsgAvailable(){
    while(Serial.available()){
        char ch = (char) Serial.read();
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

Msg* SerialCommunicator::receiveMsg(){
    if(availableMsg){
        Msg* msg = currentMsg;
        availableMsg = false;
        currentMsg = NULL;
        content = "";
        return msg;
    }
    return NULL;
}

