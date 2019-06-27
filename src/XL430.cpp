//
// Created by trotfunky on 10/09/18.
//

#include "XL430.h"

// Definition of previously declared static members to prevent conflicts during linking

const DynamixelAccessData& XL430::ID = DynamixelAccessData(0x07,0x00,1);
const DynamixelAccessData& XL430::LED = DynamixelAccessData(0x41,0x00,1);
const DynamixelAccessData& XL430::torqueEnable = DynamixelAccessData(0x40,0x00,1);
const DynamixelAccessData& XL430::currentTorque = DynamixelAccessData(126,0x00,2);
const DynamixelAccessData& XL430::goalAngle = DynamixelAccessData(0x74,0x00,4);
const DynamixelAccessData& XL430::currentAngle = DynamixelAccessData(0x84,0x00,4);
const DynamixelAccessData& XL430::goalVelocity = DynamixelAccessData(0x68,0x00,4);
const DynamixelAccessData& XL430::currentVelocity = DynamixelAccessData(0x80,0x00,4);
const DynamixelAccessData& XL430::operatingMode = DynamixelAccessData(0x0B,0x00,1);
const DynamixelAccessData& XL430::velocityLimit = DynamixelAccessData(112,0x00,4);
const DynamixelAccessData& XL430::moving = DynamixelAccessData(122,0x00,1);
const DynamixelAccessData& XL430::movingStatus = DynamixelAccessData(123,0x00,1);
const DynamixelAccessData& XL430::returnDelay = DynamixelAccessData(9,0x00,1);
const DynamixelAccessData& XL430::hardwareError = DynamixelAccessData(70,0x00,1);
const DynamixelAccessData& XL430::movingThreshold = DynamixelAccessData(24,0x00,4);
const DynamixelAccessData& XL430::movingOffset = DynamixelAccessData(20,0x00,4);
const DynamixelAccessData& XL430::watchdog = DynamixelAccessData(98,0x00,1);

XL430::XL430(uint8_t id, const DynamixelPacketSender& dynamixelManager) : DynamixelMotor(id, DynamixelMotorData(id, ID,
                                          LED,torqueEnable,currentTorque, goalAngle, currentAngle,
                                          goalVelocity, currentVelocity,
                                          operatingMode,
                                          torqueConversionFactor,
                                          angleConversionFactor,velocityConversionFactor), dynamixelManager)
{

}

DynamixelPacketData* XL430::makeWritePacket(DynamixelAccessData accessData, char *parameters)
{
    uint8_t packetSize = (uint8_t)dynamixelV2::minPacketLength+accessData.length;
    char* packet = manager.txBuffer;

    int position = 0;
    for(uint8_t i : v2Header)
    {
        packet[position] = i;
        position ++;
    }

    packet[position] = motorData.motorID;
    motorData.motorID = motorID;            // This allows to change motor ID and keep communicating
    position ++;
    uint16_t instructionsLength = (uint8_t)dynamixelV2::minWriteInstructionLength+accessData.length;
    packet[position] = instructionsLength & 0xFF;
    position++;
    packet[position] = (instructionsLength >> 8) & 0xFF;
    position++;
    packet[position] = (uint8_t)dynamixelV2::writeInstruction;
    position++;
    packet[position] = accessData.address[0];
    position++;
    packet[position] = accessData.address[1];
    position++;

    for(int i = 0;i<accessData.length;i++)
    {
        packet[position] = parameters[i];
        position++;
    }

    unsigned short crc = crc_compute(packet,instructionsLength+(uint8_t)dynamixelV2::headerLength);
    packet[position] = crc & 0xFF;
    position++;
    packet[position] = (crc >> 8) & 0xFF;

    return(new DynamixelPacketData(packetSize,(uint8_t)dynamixelV2::statusResponseLength));
}


DynamixelPacketData* XL430::makeReadPacket(DynamixelAccessData accessData)
{
    uint8_t packetSize = (uint8_t)dynamixelV2::minPacketLength+2;
    char* packet = manager.txBuffer;
    unsigned int position = 0;
    // HEADER
    for(unsigned char headerPart : v2Header) {
        packet[position++] = headerPart;
    }

    // Packet ID
    packet[position++] = motorData.motorID;
    motorID = motorData.motorID;

    // Instruction Length
    packet[position++] = (uint8_t)dynamixelV2::readInstructionLength;
    packet[position++] = 0;

    // Instruction
    packet[position++] = (uint8_t)dynamixelV2::readInstruction;
    // Params: Start, Length
    packet[position++] = accessData.address[0];
    packet[position++] = accessData.address[1];

    packet[position++] = accessData.length & 0xFF;
    packet[position++] = (accessData.length >> 8) & 0xFF;

    unsigned short crc = crc_compute(packet, packetSize-2);
    packet[position++] = crc & 0xFF;
    packet[position++] = (crc >> 8) & 0xFF;

    return(new DynamixelPacketData(packetSize, 11+accessData.length));
}

bool XL430::decapsulatePacket(const char *packet)
{
    unsigned short responseLength = (uint8_t)dynamixelV2::minResponseLength + packet[(uint8_t)dynamixelV2::lengthLSBPos] + (packet[(uint8_t)dynamixelV2::lengthMSBPos] << 8);

    // Checks CRC
    if(crc_compute(packet,responseLength) == (packet[responseLength]+(packet[responseLength+1] << 8)))
    {
        // If valid, checks instruction type
        return((int)packet[(uint8_t)dynamixelV2::instructionPos] == (uint8_t)dynamixelV2::statusInstruction);
    }
    return(false);
}

bool XL430::checkAlert(const char *packet) {
    if(!((uint8_t)packet[(uint8_t)dynamixelV2::responseErrorPos] & (uint8_t)dynamixelV2::alertBit)) {
        return(true);
    }
    return(false);
}

bool XL430::decapsulatePacket(const char *packet, float &value)
{
    if(decapsulatePacket(packet))
    {
        int parameterLength = packet[(uint8_t)dynamixelV2::lengthLSBPos] + (packet[(uint8_t)dynamixelV2::lengthMSBPos] << 8) - 4;

        for(int i = 0; i<parameterLength; i++)
        {
            value += (int)(packet[(uint8_t)dynamixelV2::responseParameterStart+i] << 8*i);
        }

        return(true);
    }
    else
    {
        value = 0;
        return(false);
    }
}

bool XL430::decapsulatePacket(const char *packet, int &value)
{
    if(decapsulatePacket(packet))
    {
        int parameterLength = packet[(uint8_t)dynamixelV2::lengthLSBPos] + (packet[(uint8_t)dynamixelV2::lengthMSBPos] << 8) - (uint8_t)dynamixelV2::nonParameterBytesLength;

        for(int i = 0; i<parameterLength; i++)
        {
            value += (int)(packet[(uint8_t)dynamixelV2::responseParameterStart+i] << 8*i);
        }

        return(true);
    }
    else
    {
        value = 0;
        return(false);
    }
}

void XL430::reboot() {
    uint8_t packetSize = 10;
    char* packet = manager.txBuffer;
    unsigned int position = 0;
    // HEADER
    for(unsigned char headerPart : v2Header) {
        packet[position++] = headerPart;
    }

    // Packet ID
    packet[position++] = motorData.motorID;
    motorID = motorData.motorID;

    // Instruction Length
    packet[position++] = 0x03;
    packet[position++] = 0x00;

    // Instruction
    packet[position++] = (uint8_t)dynamixelV2::rebootInstruction;

    unsigned short crc = crc_compute(packet, packetSize-2);
    packet[position++] = crc & 0xFF;
    packet[position++] = (crc >> 8) & 0xFF;

    char* answer = manager.sendPacket(new DynamixelPacketData(packetSize, 11));
    bool valid = decapsulatePacket(answer);

}

DynamixelMotor* XL430GeneratorFunction(uint8_t id, DynamixelPacketSender* packetSender) {
    return new XL430(id, *packetSender);
}