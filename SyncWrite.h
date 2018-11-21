//
// Created by jglrxavpok on 18/11/18.
//

#ifndef PROTOBRAS_SYNCWRITE_H
#define PROTOBRAS_SYNCWRITE_H


#include "DynamixelUtils.h"
#include "DynamixelManager.h"

/**
 * This class represents a Sync Write instruction. It is mutable to avoid reallocating objects and because the number of motors is unlikely to change during runtime
 */
class SyncWrite {

public:
    SyncWrite(const DynamixelManager &, unsigned int, uint16_t, uint16_t);

    /**
     * Sets up the motor IDs in the chain
     */
    void setMotorID(unsigned int, uint8_t);

    /**
     * Sets data for a given motor. The length read from the 'data' pointer is the same given in the constructor
     */
    void setData(unsigned int, char*);

    /**
     * Creates the packet for sending (in DynamixelManager#txBuffer !!)
     * @return
     */
    DynamixelPacketData* preparePacket();

    /**
     * Helper method to directly send a new packet to the manager
     * @return
     */
    bool send();

private:
    DynamixelManager manager;

    /**
     * Start address of area to write
     */
    const uint16_t address;
    /**
     * Length of data to write (per motor)
     */
    const uint16_t length;
    /**
     * Number of motors in chain
     */
    const unsigned int motorCount;
    /**
     * The ids of the motors in chain
     */
    uint8_t* motors;
    /**
     * The data to write to each motor
     */
    char* rawData;
};


#endif //PROTOBRAS_SYNCWRITE_H
