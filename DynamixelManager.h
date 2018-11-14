//
// Created by trotfunky on 07/09/18.
//

#ifndef DYN_MANAGER_H
#define DYN_MANAGER_H

#include "Arduino.h"
#include "DynamixelUtils.h"

// TODO : Rajouter les vraies fonctions de manager

//!High-level DynamixelMotor interface
/*!
 * The first goal of the DynamixelManager is to provide a way for every DynamixelMotor to communicate.
 * It provides the sendPacket() function which sends a DynamixelPacket and returns the response.
 * <br>The second goal of the DynamixelManager is to provide a high-level interface to use DynamixelMotor objects :
 * \li Motor instantiation and ID conflict prevention
 * \li Motor control by ID
 */
class DynamixelManager {

public:

    DynamixelManager(HardwareSerial*);


    /*!
     * Sends DynamixelPacket through serial port and returns response string if there should be one.
     * \warning If serial RX is not connected or DynamixelPacket::responseSize is too big, serial might take
     * some time - up to several seconds - before timing out and returning unpredictable data.
     * @param packet
     * @return Response string, eventually empty.
     */
    char* sendPacket(DynamixelPacketData *) const;

    char* rxBuffer;
    char* txBuffer;
private:

    HardwareSerial* serial;
};

#endif //DYN_MANAGER_H
