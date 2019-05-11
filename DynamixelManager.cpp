//
// Created by trotfunky on 07/09/18.
//

#include "DynamixelManager.h"

// TODO : Try to generalize for different baudrates and serials
DynamixelManager::DynamixelManager(HardwareSerial* dynamixelSerial, Stream* debugSerial, long baudrate) : serial(dynamixelSerial), debugSerial(debugSerial)
{
    txBuffer = new char[30];
    rxBuffer = new char[30];

    serial->begin(baudrate);

    setHalfDuplex(*serial);

    serial->setTimeout(50);
    setReadMode(*serial);
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

char* DynamixelManager::readPacket(uint8_t responseSize) const
{
    memset(rxBuffer, 0, responseSize);

    if(responseSize == 0 )
    {
        return nullptr;
    }
    else
    {
        if(serial->readBytes(rxBuffer,responseSize) == 0) {
            delay(100);
#ifdef DYN_VERBOSE
            if(debugSerial) {
                debugSerial->printf("[Dynamixel-Com] Nothing to read (expecting %i) ...\n", responseSize);
            }
#endif
        }
#ifdef DYN_VERBOSE
        if(debugSerial) {
            debugSerial->printf("[Dynamixel-Com] Received (%i): ",responseSize);
            for(unsigned int i = 0; i < responseSize; i++)
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

char* DynamixelManager::sendPacket(DynamixelPacketData* packet) const
{
    // vide le buffer de réception en cas d'erreur de transmission lors de l'échange précédent
    while(serial->available())
        serial->read();

    this->setWriteMode(*serial);
    debugSerial->printf("[Dynamixel-Com] Available for writing is %i\n", serial->availableForWrite());
    serial->write(txBuffer,packet->dataSize);       // Sends buffered packet

#ifdef DYN_VERBOSE
    if(debugSerial) {
        debugSerial->printf("[Dynamixel-Com] Sent (%i): ",packet->dataSize);
        for(int i = 0;i<packet->dataSize;i++)
        {
            debugSerial->print((int)(txBuffer[i]));
            debugSerial->print(",");
        }
        debugSerial->println("");
    }
#endif
    serial->flush();
    serial->clear();
#ifdef DYN_VERBOSE
    if(debugSerial) {
        debugSerial->printf("[Dynamixel-Com] Sent (read from Serial) (%i): ",packet->dataSize);
        for(int i = 0;i<packet->dataSize;i++)
        {
            debugSerial->print((int)(txBuffer[i]));
            debugSerial->print(",");
        }
        debugSerial->println("");
    }
#endif

    memset(txBuffer,0,packet->dataSize);            // Clears transmission buffer
    uint8_t responseSize = packet->responseSize;
    delete packet;
    this->setReadMode(*serial);

    return readPacket(responseSize);
}