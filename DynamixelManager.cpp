//
// Created by trotfunky on 07/09/18.
//

#include "DynamixelManager.h"

// TODO : Try to generalize for different baudrates and serials
DynamixelManager::DynamixelManager(HardwareSerial* dynamixelSerial) : serial(dynamixelSerial)
{
    serial->begin(57600);
}

String DynamixelManager::sendPacket(DynamixelPacket *packet) const
{

#ifdef DYN_VERBOSE
    for(int i = 0;i<packet->packetSize;i++)
    {
        Serial.print(*(packet->packetData+i));
        Serial.print(",");
    }
    Serial.println("");
#endif

    serial->write(packet->data,packet->dataSize);       // Sends packet

    char* clearBuffer = new char[packet->dataSize];
    serial->readBytes(clearBuffer,packet->dataSize);      // Reads sent packet to clear serial


    if(packet->responseSize == 0 )
    {
        delete packet;
        return String();
    }
    else
    {
        String response = serial->readString(packet->responseSize);

        delete packet;

#ifdef DYN_VERBOSE
        Serial.println("Received: ");
        for(unsigned int i = 0; i < response.length(); i++)
        {
            Serial.print((int)response[i]);
            Serial.print(",");
        }
        Serial.println("");
#endif

        return(response);
    }
}