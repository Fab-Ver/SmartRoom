#ifndef __MODULE_BT__
#define __MODULE_BT__

#include "Arduino.h"
#include "SoftwareSerial.h"
#include "Msg.h"

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

        /**
         * Send a message in the BT channel.
         * @param msg the message to be sent. 
        */
        void sendMsg(Msg msg);
};

#endif