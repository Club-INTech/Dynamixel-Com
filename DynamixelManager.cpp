//
// Created by trotfunky on 07/09/18.
//

#include "DynamixelManager.h"

// TODO : Try to generalize for different baudrates and serials
DynamixelManager::DynamixelManager(HardwareSerial* dynamixelSerial, HardwareSerial* debugSerial = NULL) : serial(dynamixelSerial), debugSerial(debugSerial)
{
    txBuffer = new char[30];
    rxBuffer = new char[30];

    serial->begin(57600);
}

char* DynamixelManager::sendPacket(DynamixelPacketData* packet) const
{

    serial->write(txBuffer,packet->dataSize);       // Sends buffered packet
    
#ifdef DYN_VERBOSE
    if(debugSerial) {
        debugSerial->printf("Sent (%i):\n",packet->dataSize);
        for(int i = 0;i<packet->dataSize;i++)
        {
            debugSerial->print((int)(txBuffer[i]));
            debugSerial->print(",");
        }
        debugSerial->println("");
    }
#endif

    serial->readBytes(txBuffer,packet->dataSize);   // Reads sent packet to clear serial

    memset(rxBuffer, 0, packet->responseSize); // TODO: remove, only for testing

    memset(txBuffer,0,packet->dataSize);            // Clears transmission buffer


    if(packet->responseSize == 0 )
    {
        delete packet;
        return nullptr;
    }
    else
    {

        serial->readBytes(rxBuffer,packet->responseSize);

        delete packet;

#ifdef DYN_VERBOSE
        if(debugSerial) {
            debugSerial->printf("Received (%i):\n",packet->responseSize);
            for(unsigned int i = 0; i < packet->responseSize; i++)
            {
                debugSerial->print((int)rxBuffer[i]);
                debugSerial->print(",");
            }
            debugSerial->println("");
        }
#endif

        return(rxBuffer);
    }
}