#ifndef __COMMUNICATOR__
#define __COMMUNICATOR__

#include "Msg.h"

class Communicator {
    public:
        /**
         * Init the communication. 
        */
        virtual void init() = 0;

        /**
         * Check if any message is available.
         * @return true if the message is available, false otherwise. 
        */
        virtual bool isMsgAvailable() = 0;

        /**
         * Receive the current message.
         * @return the message if available, NULL otherwise. 
        */
        virtual Msg* receiveMsg() = 0;

        /**
         * Send a message in the BT channel.
         * @param msg the message to be sent. 
        */
         virtual void sendMsg(Msg msg) = 0;
};

#endif