//
// Created by trotfunky on 07/09/18.
//

#include "DynamixelManager.h"

// TODO : Try to generalize for different baudrates and serials
DynamixelManager::DynamixelManager(HardwareSerial* dynamixelSerial) : serial(dynamixelSerial)
{
    txBuffer = new char[30];
    rxBuffer = new char[30];

    serial->begin(57600);
}

char* DynamixelManager::sendPacket(DynamixelPacketData* packet) const
{

    serial->write(txBuffer,packet->dataSize);       // Sends buffered packet

#ifdef DYN_VERBOSE
    Serial2.printf("Sent (%i):\n",packet->dataSize);
    for(int i = 0;i<packet->dataSize;i++)
    {
        Serial2.print((int)(txBuffer[i]));
        Serial2.print(",");
    }
    Serial2.println("");
#endif

    serial->readBytes(txBuffer,packet->dataSize);   // Reads sent packet to clear serial

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
        Serial2.printf("Received (%i):\n",packet->responseSize);
        for(unsigned int i = 0; i < packet->responseSize; i++)
        {
            Serial2.print((int)rxBuffer[i]);
            Serial2.print(",");
        }
        Serial2.println("");
#endif

        return(rxBuffer);
    }
}