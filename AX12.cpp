//
// Created by trotfunky on 16/04/19.
//

#include "AX12.h"

const DynamixelAccessData& AX12::ID = DynamixelAccessData(0x03,0x00,1);
const DynamixelAccessData& AX12::LED = DynamixelAccessData(0x19,0x00,1);
const DynamixelAccessData& AX12::torqueEnable = DynamixelAccessData(0x18,0x00,1);
const DynamixelAccessData& AX12::currentTorque = DynamixelAccessData(0x28,0x00,2);
const DynamixelAccessData& AX12::goalAngle = DynamixelAccessData(0x1e,0x00,2);
const DynamixelAccessData& AX12::currentAngle = DynamixelAccessData(0x24,0x00,2);
const DynamixelAccessData& AX12::goalVelocity = DynamixelAccessData(0x26,0x00,2);
const DynamixelAccessData& AX12::currentVelocity = DynamixelAccessData(0x20,0x00,4);
const DynamixelAccessData& AX12::operatingMode = DynamixelAccessData(0xff,0x00,1);  // FIXME : To change, does not work like that

const DynamixelAccessData& AX12::returnDelay = DynamixelAccessData(5,0x00,1);
const DynamixelAccessData& AX12::velocityLimit = DynamixelAccessData(32,0x00,2);
const DynamixelAccessData& AX12::moving = DynamixelAccessData(46,0x00,2);
const DynamixelAccessData& AX12::movingStatus = DynamixelAccessData(0xFF,0x00,2);  // FIXME : To change, does not work like that
const DynamixelAccessData& AX12::hardwareError = DynamixelAccessData(0xFF,0x00,2);  // FIXME : To change, does not work like that
const DynamixelAccessData& AX12::movingThreshold = DynamixelAccessData(0xFF,0x00,2);  // FIXME : To change, does not work like that


AX12::AX12(uint8_t id, const DynamixelPacketSender& dynamixelManager) : DynamixelMotor(id,DynamixelMotorData(id, ID,
                LED, torqueEnable, currentTorque, goalAngle, currentAngle, goalVelocity,
                currentVelocity, operatingMode, torqueConversionFactor, angleConversionFactor, velocityConversionFactor),
                dynamixelManager)
{

}

DynamixelPacketData* AX12::makeWritePacket(DynamixelAccessData accessData, char* parameters)
{
    uint8_t packetSize = (uint8_t)dynamixelV1::minPacketLength+accessData.length;
    char* packet = manager.txBuffer;

    int position = 0;
    for(uint8_t i : v1Header)
    {
        packet[position] = i;
        position ++;
    }

    packet[position] = motorData.motorID;
    motorData.motorID = motorID;            // This allows to change motor ID and keep communicating
    position ++;
    uint8_t instructionsLength = (uint8_t)dynamixelV1::minWriteInstructionLength+accessData.length;
    packet[position] = instructionsLength & 0xFF;
    position++;
    packet[position] = (uint8_t)dynamixelV1::writeInstruction;
    position++;
    packet[position] = accessData.address[0];
    position++;

    for(int i = 0;i<accessData.length;i++)
    {
        packet[position] = parameters[i];
        position++;
    }

    unsigned short chksm = v1Checksum(packet,instructionsLength+(uint8_t)dynamixelV1::headerLength);
    packet[position] = chksm & 0xFF;

    return(new DynamixelPacketData(packetSize,(uint8_t)dynamixelV1::statusResponseLength));
}

// TODO AX12::makeReadPacket
DynamixelPacketData* AX12::makeReadPacket(DynamixelAccessData)
{
    return nullptr;
}

// TODO
bool AX12::decapsulatePacket(const char *) {
    return false;
}
bool AX12::decapsulatePacket(const char *, float &) {
    return false;
}
bool AX12::decapsulatePacket(const char *, int &) {
    return false;
}

DynamixelMotor* AX12GeneratorFunction(uint8_t id, DynamixelPacketSender* packetSender) {
    return new AX12(id, *packetSender);
}