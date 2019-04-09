//
// Created by jglrxavpok on 01/04/19.
//

#ifndef DYNAMIXEL_COM_SYNCREAD_H
#define DYNAMIXEL_COM_SYNCREAD_H

#include "DynamixelManager.h"

/**
 * This class represents a Sync Read instruction. It is mutable to avoid reallocating objects and because the number of motors is unlikely to change during runtime
 */
class SyncRead {
public:
    SyncRead(const DynamixelManager &, unsigned int, uint16_t, uint16_t);
    SyncRead(const DynamixelManager &, unsigned int, const DynamixelAccessData& data);

    /**
     * Sets up the motor IDs in the chain
     */
    void setMotorID(unsigned int, uint8_t);

    /**
 * Creates the packet for sending (in DynamixelManager#txBuffer !!)
 * @return
 */
    DynamixelPacketData* preparePacket();

    /**
     * Send a Sync Read instruction and read the answers into the given buffer.
     * Struction example with 2 motors:
     * [Motor at Index 0, Byte 0 | Motor at Index 0, Byte 1 | Motor at Index 1, Byte 0 | Motor at Index 1, Byte 1]
     * @return
     */
    bool read(char*);

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

};


#endif //DYNAMIXEL_COM_SYNCREAD_H
