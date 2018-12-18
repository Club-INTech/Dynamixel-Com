//
// Created by jglrxavpok on 18/12/18.
//

#ifndef _DYNAMIXELPACKETSENDER_H
#define _DYNAMIXELPACKETSENDER_H

#include "DynamixelUtils.h"

class DynamixelPacketSender {

public:
    char* txBuffer;
    char* rxBuffer;
    virtual char* sendPacket(DynamixelPacketData *) const = 0;

private:

};


#endif //_DYNAMIXELPACKETSENDER_H
