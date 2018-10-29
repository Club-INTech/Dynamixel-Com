//
// Created by trotfunky on 07/09/18.
//

#include "DynamixelManager.h"

DynamixelManager::DynamixelManager(HardwareSerial* dynamixelSerial) : serial(dynamixelSerial)
{
    serial->begin(57600);
}

std::string DynamixelManager::sendPacket(DynamixelPacket *packet) const
{
//    for(int i = 0;i<packet->packetSize;i++)
//    {
//        Serial.print(*(packet->packetData+i));
//        Serial.print(",");
//    }
    serial->write(packet->packetData,packet->packetSize);

    char clearBuffer[30] = {0};
    serial->readBytes(clearBuffer,packet->packetSize);


    if(packet->responseSize == 0 )
    {
        delete packet;
        return std::string();
    }
    else
    {
        char* buffer = new char[packet->responseSize+1];
        serial->readBytes(buffer,packet->responseSize+1);
        std::string response (buffer,packet->responseSize+1);

        delete[] buffer;
        delete packet;

        Serial.println("Received: ");
        for(unsigned int i = 0; i < response.length(); i++)
        {
            Serial.print((int)response[i]);
            Serial.print(",");
        }
        Serial.println("");

        return(response);
    }
}