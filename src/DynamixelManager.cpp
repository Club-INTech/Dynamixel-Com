//
//

#include "DynamixelManager.h"

DynamixelManager::DynamixelManager(Stream* debugSerial) : debugSerial(debugSerial)
{
    txBuffer = new char[30];
    rxBuffer = new char[30];
}

// TODO : Try to generalize for different baudrates and serials
DynamixelManager::DynamixelManager(HardwareSerial* dynamixelSerial, Stream* debugSerial, long baudrate) : DynamixelManager(debugSerial)
{
#if defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__) || defined(__MK64FX512__) || defined(__MK66FX1M0__)
    serial = dynamixelSerial;
    ((HardwareSerial*)serial)->begin(baudrate);

    setHalfDuplex(*serial);

    serial->setTimeout(50);
    setReadMode(*serial);
#else
    pinMode(LED_BUILTIN,OUTPUT);
    while (true)
    {
        digitalWrite(LED_BUILTIN,HIGH);
        delay(250);
        digitalWrite(LED_BUILTIN,LOW);
        delay(250);
        Serial.println("HardwareSerial not supported on this board. Supported boards:\n - Teensy 3.x");
    }
#endif
}

DynamixelManager::DynamixelManager(int pin_RX, int pin_TX, Stream* debugSerial, long baudrate) : DynamixelManager(debugSerial)
{
    TX = pin_TX;
    RX = pin_RX;

    serial = new SoftwareSerial(RX, TX);
    ((SoftwareSerial*)serial)->begin(baudrate);
    serial->setTimeout(50);
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
#ifdef DYN_VERBOSE
    debugSerial->printf("[Dynamixel-Com] Available for writing is %i\n", serial->availableForWrite());
#endif
    serial->write((uint8_t)0);  // Garbage byte: SoftwareSerial Half-duplex does not send the first byte.
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