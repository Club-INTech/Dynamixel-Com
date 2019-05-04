//
// Created by jglrxavpok on 18/11/18.
//

#include "DynamixelUtils.h"
#include "SyncWrite.h"
#include "DynamixelManager.h"

SyncWrite::SyncWrite(const DynamixelManager& manager, const unsigned int motorCount, const DynamixelAccessData& data, uint8_t protocol): manager(manager), motorCount(motorCount), address((uint16_t ) (data.address[0] | (data.address[1] << 8))), length(data.length), protocol(protocol) {
    motors = new uint8_t[motorCount];
    rawData = new char[motorCount*length];
}

SyncWrite::SyncWrite(const DynamixelManager& manager, const unsigned int motorCount, const uint16_t address, const uint16_t length, uint8_t protocol): manager(manager), motorCount(motorCount), address(address), length(length), protocol(protocol) {
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
    uint8_t packetSize = 0;
    if(protocol == 2) {
        unsigned int instrLength =  2 /* CRC */ + 2 /* Address */ + 2 /* Length */ + 1 /* Instruction */ + (length+1)*motorCount /* Data */;
        packetSize = (uint8_t) (instrLength + 4 /* header*/ + 1 /* id */ + 2 /* packet length */);
        unsigned int position = 0;
        for(char headerPart : v2Header) {
            packet[position++] = headerPart;
        }

        packet[position++] = (uint8_t)dynamixelV2::broadcastId;
        packet[position++] = instrLength & 0xFF;
        packet[position++] = (instrLength >> 8) & 0xFF;
        packet[position++] = (uint8_t)dynamixelV2::syncWriteInstruction;

        packet[position++] = address & 0xFF;
        packet[position++] = (address >> 8) & 0xFF;

        packet[position++] = length & 0xFF;
        packet[position++] = (length >> 8) & 0xFF;

        for(unsigned int motorIndex = 0; motorIndex < motorCount; motorIndex++) {
            packet[position++] = motors[motorIndex]; // write motor ID
            for (unsigned int index = 0; index < length; ++index) { // then write data
                packet[position++] = rawData[index+motorIndex*length];
            }
        }

        unsigned short crc = crc_compute(packet,packetSize-2);
        packet[position++] = crc & 0xFF;
        packet[position++] = (crc >> 8) & 0xFF;
    } else if (protocol == 1) {
        unsigned int instrLength =  1 /* Instruction */ + 0 /* length */ + 1 /* starting address */ + 1 /* length of data */ + motorCount /* IDs */ + length*motorCount /* data */ + 1 /* checksum */;
        packetSize = (uint8_t) (instrLength + 2 /* header*/ + 1 /* id */ + 1 /* packet length */);
        unsigned int position = 0;
        for(char headerPart : v1Header) {
            packet[position++] = headerPart;
        }

        packet[position++] = (uint8_t)dynamixelV1::broadcastId;
        packet[position++] = instrLength & 0xFF;
        packet[position++] = (uint8_t) dynamixelV1 ::syncWriteInstruction;
//        packet[position++] = (length+1)*motorCount+4;
        packet[position++] = address & 0xFF;
        packet[position++] = length & 0xFF;

        for(unsigned int motorIndex = 0; motorIndex < motorCount; motorIndex++) {
            packet[position++] = motors[motorIndex]; // write motor ID
            for (unsigned int index = 0; index < length; ++index) { // then write data
                packet[position++] = rawData[index+motorIndex*length];
            }
        }

        uint8_t checksum = v1Checksum(packet, instrLength+(uint8_t)dynamixelV1::headerLength);
        packet[position++] = checksum & 0xFF;
    }

    return(new DynamixelPacketData(packetSize, 0));
}

bool SyncWrite::send() {
    char* returnPacket = manager.sendPacket(preparePacket());
    return false;// TODO return decapsulatePacket(returnPacket);
}