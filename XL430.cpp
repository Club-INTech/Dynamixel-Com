//
// Created by trotfunky on 10/09/18.
//

#include "XL430.h"

// Definition of previously declared static members to prevent conflicts during linking

const DynamixelAccessData& XL430::xl430ID = DynamixelAccessData(0x07,0x00,1);
const DynamixelAccessData& XL430::xl430LED = DynamixelAccessData(0x41,0x00,1);
const DynamixelAccessData& XL430::xl430TorqueEnable = DynamixelAccessData(0x40,0x00,1);
const DynamixelAccessData& XL430::xl430CurrentTorque = DynamixelAccessData(0x7e,0x00,2);
const DynamixelAccessData& XL430::xl430GoalAngle = DynamixelAccessData(0x74,0x00,4);
const DynamixelAccessData& XL430::xl430CurrentAngle = DynamixelAccessData(0x84,0x00,4);
const DynamixelAccessData& XL430::xl430GoalVelocity = DynamixelAccessData(0x68,0x00,4);
const DynamixelAccessData& XL430::xl430CurrentVelocity = DynamixelAccessData(0x80,0x00,4);

XL430::XL430(uint8_t id, const DynamixelManager& dynamixelManager) : DynamixelMotor(id, DynamixelMotorData(id, xl430ID,
                                          xl430LED,xl430TorqueEnable,xl430CurrentTorque, xl430GoalAngle, xl430CurrentAngle,
                                          xl430GoalVelocity,xl430CurrentVelocity, torqueConversionFactor,
                                          angleConversionFactor,velocityConversionFactor), dynamixelManager)
{

}

DynamixelPacketData* XL430::makeWritePacket(DynamixelAccessData accessData, char *parameters)
{
    uint8_t packetSize = dynamixelV2::minPacketLength+accessData.length;
    char* packet = manager.txBuffer;

    int position = 0;
    for(int i=0;i<4;i++)
    {
        packet[position] = v2Header[i];
        position ++;
    }

    packet[position] = motorData.motorID;
    motorData.motorID = motorID;            // This allows to change motor ID and keep communicating
    position ++;
    int instructionsLength = dynamixelV2::minInstructionLength+accessData.length;
    packet[position] = instructionsLength & 0xFF;
    position++;
    packet[position] = (instructionsLength >> 8) & 0xFF;
    position++;
    packet[position] = dynamixelV2::writeInstruction;
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

    unsigned short crc = crc_compute(packet,instructionsLength+5);
    packet[position] = crc & 0xFF;
    position++;
    packet[position] = (crc >> 8) & 0xFF;

    return(new DynamixelPacketData(packetSize,11));
}


DynamixelPacketData* XL430::makeReadPacket(DynamixelAccessData accessData)
{
    uint8_t packetSize = dynamixelV2::minPacketLength+2;
    char* packet = manager.txBuffer;
    unsigned int position = 0;
    // HEADER
    for(unsigned char headerPart : v2Header) {
        packet[position++] = headerPart;
    }

    // Packet ID
    packet[position++] = motorData.motorID;
    motorID = motorData.motorID;

    // Instruction Length: 4(params) +3
    packet[position++] = 7;
    packet[position++] = 0;

    // Instruction
    packet[position++] = dynamixelV2::readInstruction;
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
    unsigned short responseLength = dynamixelV2::minResponseLength + packet[dynamixelV2::lengthLSBPos] + (packet[dynamixelV2::lengthMSBPos] << 8);

    // Checks CRC
    if(crc_compute(packet,responseLength) == (packet[responseLength]+(packet[responseLength+1] << 8)))
    {
        // If valid, checks alert byte and instruction type
        if(!((uint8_t)packet[8] & dynamixelV2::alertBit) && (int)packet[dynamixelV2::instructionPos] == dynamixelV2::statusInstruction)
        {
            return(true);
        }
    }
    return(false);
}


bool XL430::decapsulatePacket(const char *packet, float &value)
{
    if(decapsulatePacket(packet))
    {
        int parameterLength = packet[dynamixelV2::lengthLSBPos] + (packet[dynamixelV2::lengthMSBPos] << 8) - 4;

        for(int i = 0; i<parameterLength; i++)
        {
            value += (int)(packet[dynamixelV2::responseParameterStart+i] << 8*i);
        }

        return(true);
    }
    else
    {
        value = 0;
        return(false);
    }
}
