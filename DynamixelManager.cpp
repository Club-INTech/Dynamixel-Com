//
// Created by trotfunky on 07/09/18.
//

#include "DynamixelManager.h"

// TODO : Try to generalize for different baudrates and serials
DynamixelManager::DynamixelManager(HardwareSerial* dynamixelSerial) : serial(dynamixelSerial)
{
    serial->begin(57600);
}

std::string DynamixelManager::sendPacket(DynamixelPacket *packet) const
{

#ifdef DYN_VERBOSE
    for(int i = 0;i<packet->packetSize;i++)
    {
        Serial.print(*(packet->packetData+i));
        Serial.print(",");
    }
    Serial.println("");
#endif

    serial->write(packet->packet,packet->packetSize);       // Sends packet

    char clearBuffer[30] = {0};
    serial->readBytes(clearBuffer,packet->packetSize);      // Reads sent packet to clear serial


    if(packet->responseSize == 0 )
    {
        delete packet;
        return std::string();
    }
    else
    {
        char* buffer = new char[packet->responseSize+1];
        serial->readBytes(buffer,packet->responseSize+1);       // Reads return message and turns it into a std::string
        std::string response (buffer,packet->responseSize+1);

        delete[] buffer;
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