//
// Created by trotfunky on 16/04/19.
//

#include "AX12.h"

const DynamixelAccessData& AX12::ax12ID = DynamixelAccessData(0x03,0x00,1);
const DynamixelAccessData& AX12::ax12LED = DynamixelAccessData(0x19,0x00,1);
const DynamixelAccessData& AX12::ax12TorqueEnable = DynamixelAccessData(0x18,0x00,1);
const DynamixelAccessData& AX12::ax12CurrentTorque = DynamixelAccessData(0x28,0x00,2);
const DynamixelAccessData& AX12::ax12GoalAngle = DynamixelAccessData(0x1e,0x00,2);
const DynamixelAccessData& AX12::ax12CurrentAngle = DynamixelAccessData(0x24,0x00,2);
const DynamixelAccessData& AX12::ax12GoalVelocity = DynamixelAccessData(0x26,0x00,2);
const DynamixelAccessData& AX12::ax12CurrentVelocity = DynamixelAccessData(0x20,0x00,4);
const DynamixelAccessData& AX12::ax12OperatingMode = DynamixelAccessData(0xff,0x00,1);  // FIXME : To change, does not work like that

AX12::AX12(uint8_t id, const DynamixelPacketSender& dynamixelManager) : DynamixelMotor(id,DynamixelMotorData(id, ax12ID,
                ax12LED, ax12TorqueEnable, ax12CurrentTorque, ax12GoalAngle, ax12CurrentAngle, ax12GoalVelocity,
                ax12CurrentVelocity, ax12OperatingMode, torqueConversionFactor, angleConversionFactor, velocityConversionFactor),
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

