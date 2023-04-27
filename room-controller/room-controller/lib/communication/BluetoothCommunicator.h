#ifndef __BLUETOOTH_COMMUNICATOR__
#define __BLUETOOTH_COMMUNICATOR__

#include <Arduino.h>
#include "SoftwareSerial.h"
#include "Communicator.h"

class BluetoothCommunicator : public Communicator {
    private:
        SoftwareSerial* channel;
        Msg* currentMsg;
        bool availableMsg; 
        String content;
    public:
        BluetoothCommunicator(int rxPin, int txPin);
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