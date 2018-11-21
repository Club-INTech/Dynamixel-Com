//
// Created by jglrxavpok on 18/11/18.
//

#include "DynamixelUtils.h"
#include "SyncWrite.h"
#include "DynamixelManager.h"

SyncWrite::SyncWrite(const DynamixelManager& manager, const unsigned int motorCount, const uint16_t address, const uint16_t length): manager(manager), motorCount(motorCount), address(address), length(length) {
    motors = new uint8_t[motorCount];
    rawData = new char[motorCount*length];
}

void SyncWrite::setMotorID(unsigned int index, uint8_t id) {
    motors[index] = id;
}

void SyncWrite::setData(unsigned int motorIndex, char* data) {
    for(unsigned int i = 0;i<length;i++) {
        rawData[motorIndex*length+i] = data[i];
    }
}

DynamixelPacketData* SyncWrite::preparePacket() {
    char* packet = manager.txBuffer;
    uint8_t packetSize = (uint8_t) (4 + 1 + 2 + 1 + motorCount*length +2);
    unsigned int instrLength =  2 /* CRC */ + 2 /* Address */ + 2 /* Length */ + 1 /* Instruction */ + (length+1)*motorCount /* Data */;
    unsigned int position = 0;
    for(char headerPart : v2Header) {
        packet[position++] = headerPart;
    }

    packet[position++] = dynamixelV2::broadcastId;
    packet[position++] = dynamixelV2::syncWriteInstruction;
    packet[position++] = (char)(instrLength & 0xFF);
    packet[position++] = (char)((instrLength >> 8) & 0xFF);

    for(unsigned int motorIndex = 0; motorIndex < motorCount; motorIndex++) {
        packet[position++] = motors[motorIndex]; // write motor ID
        for (unsigned int index = 0; index < length; ++index) { // then write data
            packet[position++] = rawData[index+motorIndex*length];
        }
    }

    unsigned short crc = crc_compute(packet,packetSize-2);
    packet[position++] = crc & 0xFF;
    packet[position++] = (crc >> 8) & 0xFF;

    return(new DynamixelPacketData(packetSize, 11));
}

bool SyncWrite::send() {
    char* returnPacket = manager.sendPacket(preparePacket());
    return false;// TODO (decapsulatePacket(returnPacket));
}