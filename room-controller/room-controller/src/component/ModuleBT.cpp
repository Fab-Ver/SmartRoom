#include "ModuleBT.h"

ModuleBT::ModuleBT(int rxPin, int txPin){
    this->channel = new SoftwareSerial(rxPin,txPin);
}

void ModuleBT::init(){
    content.reserve(256);
    channel->begin(9600);
    availableMsg = NULL;
}

bool ModuleBT::isMsgAvailable(){
    while (channel->available()){
        char ch = (char) channel->read();
        if(ch == '\n'){
            availableMsg = new Msg(content);
            content = "";
            return true;
        } else {
            content += ch;
        }
    }
    return false;
}

Msg* ModuleBT::receiveMsg(){
    if(availableMsg != NULL){
        Msg* msg = availableMsg;
        availableMsg = NULL;
        return msg;
    } else {
        return NULL;
    }
}

void ModuleBT::sendMsg(Msg msg){
    channel->println(msg.getContent());
}

