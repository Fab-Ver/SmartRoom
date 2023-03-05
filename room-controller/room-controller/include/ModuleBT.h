#ifndef __MODULE_BT__
#define __MODULE_BT__

#include "Arduino.h"
#include "SoftwareSerial.h"

class Msg {
    private:
        String content;
    public:
        Msg(const String& content){
            this->content = content;
        }
  
        String getContent(){
            return content;
        }
};

class ModuleBT{
    private:
        SoftwareSerial* channel;
        String content;
        Msg* availableMsg;
    public:
        ModuleBT(int rxPin, int txPin);
        /**
         * Init the communication. 
        */
        void init();

        /**
         * Check if any message is available.
         * @return true if the message is available, false otherwise. 
        */
        bool isMsgAvailable();

        /**
         * Receive the current message.
         * @return the message if available, NULL otherwise. 
        */
        Msg* receiveMsg();
};

#endif