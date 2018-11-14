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

#ifdef DYN_VERBOSE
    Serial.printf("Sent (%i):\n",packet->dataSize)
    for(int i = 0;i<packet->dataSize;i++)
    {
        Serial.print(*(packet->data+i));
        Serial.print(",");
    }
    Serial.println("");
#endif

    serial->write(txBuffer,packet->dataSize);       // Sends buffered packet

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
        Serial.printf("Received (%i):\n",response.length());
        for(unsigned int i = 0; i < response.length(); i++)
        {
            Serial.print((int)response[i]);
            Serial.print(",");
        }
        Serial.println("");
#endif

        return(txBuffer);
    }
}