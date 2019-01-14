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

DynamixelMotor* DynamixelManager::createMotor(uint8_t id, MotorGeneratorFunctionType generator)
{
    DynamixelMotor* motor = generator(id, this);
    motorMap.insert({id, motor});
    return motor;
}

DynamixelMotor* DynamixelManager::getMotor(uint8_t id)
{
    return motorMap.at(id);
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
        char* buffer = new char[packet->responseSize+1];
        serial->readBytes(buffer,packet->responseSize);
        std::string response (buffer,packet->responseSize);

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