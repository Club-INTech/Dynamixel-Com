//
// Created by jglrxavpok on 01/04/19.
//

#include "SyncRead.h"

SyncRead::SyncRead(const DynamixelManager& manager, const unsigned int motorCount, const DynamixelAccessData& data): manager(manager), motorCount(motorCount), address((uint16_t ) (data.address[0] | (data.address[1] << 8))), length(data.length) {
    motors = new uint8_t[motorCount];
}

SyncRead::SyncRead(const DynamixelManager& manager, const unsigned int motorCount, const uint16_t address, const uint16_t length): manager(manager), motorCount(motorCount), address(address), length(length) {
    motors = new uint8_t[motorCount];
}

void SyncRead::setMotorID(unsigned int index, uint8_t id) {
    motors[index] = id;
}

DynamixelPacketData* SyncRead::preparePacket() {
    char* packet = manager.txBuffer;
    unsigned int instrLength =  2 /* CRC */ + 2 /* Address */ + 2 /* Length */ + 1 /* Instruction */ + motorCount /* IDs */;
    uint8_t packetSize = (uint8_t) (instrLength + 4 /* header*/ + 1 /* id */ + 2 /* packet length */);
    unsigned int position = 0;
    for(char headerPart : v2Header) {
        packet[position++] = headerPart;
    }

    packet[position++] = (uint8_t)dynamixelV2::broadcastId;
    packet[position++] = instrLength & 0xFF;
    packet[position++] = (instrLength >> 8) & 0xFF;
    packet[position++] = (uint8_t)dynamixelV2::syncReadInstruction;

    packet[position++] = address & 0xFF;
    packet[position++] = (address >> 8) & 0xFF;

    packet[position++] = length & 0xFF;
    packet[position++] = (length >> 8) & 0xFF;

    for(unsigned int motorIndex = 0; motorIndex < motorCount; motorIndex++) {
        packet[position++] = motors[motorIndex]; // write motor ID
    }

    unsigned short crc = crc_compute(packet,packetSize-2);
    packet[position++] = crc & 0xFF;
    packet[position++] = (crc >> 8) & 0xFF;

    return(new DynamixelPacketData(packetSize, 0)); // size is 0, special case as there are 'motorCount' answers
}

bool SyncRead::read(char* result) {
    uint16_t expectedPacketSize = 4/*header*/ + 1 /* ID */ + 2 /* Length */ + 1 /* Instruction */ + 1 /* Error */ + (uint16_t)length /* Parameter */ + 2 /* CRC */;
    manager.sendPacket(preparePacket());
    for(uint8_t i = 0; i < motorCount; i++) {
        char* response = manager.readPacket(expectedPacketSize);
        // TODO: check validity with CRC
        uint8_t motorID = (uint8_t)response[(uint8_t)dynamixelV2::idPos];

        // find corresponding index
        // it is possible that the packets are out of order (the datasheet makes no guarantee)
        uint8_t index = 0;
        for(uint8_t subId = 0; subId < motorCount; subId++) {
            if(motors[subId] == motorID) {
                index = subId;
                break;
            }
        }

        for (uint8_t byteIndex = 0; byteIndex < length; byteIndex++) {
            result[index*length+byteIndex] = response[(uint8_t)dynamixelV2::responseParameterStart+byteIndex];
        }
    }
    return true;// TODO return decapsulatePacket(returnPacket);
}