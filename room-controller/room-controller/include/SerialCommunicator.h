#ifndef __SERIAL_COMMUNICATOR__
#define __SERIAL_COMMUNICATOR__

#include "Msg.h"

class SerialCommunicator{
    private:
        Msg* currentMsg;
        bool availableMsg; 
        String content;
    public:
        /**
         * Initialize the communication. 
        */
        void init();

        /**
         * Check if any message is available.
         * @return true if the message is available, false otherwise.
        */
       bool isMsgAvailable();
       
       /**
        * Receive the current message.
        * @return the current message if available, NULL otherwise. 
       */
       Msg* receiveMsg();

       /**
        * Send a message in the serial line.
        * @param msg the message to be sent.
       */
       void sendMsg(Msg msg);
};

#endif