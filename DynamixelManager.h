//
// Created by trotfunky on 07/09/18.
//

#ifndef DYN_MANAGER_H
#define DYN_MANAGER_H

#include "Arduino.h"
#include "DynamixelUtils.h"
#include "DynamixelPacketSender.h"
#include "DynamixelMotor.h"
#include <map>

// TODO : Rajouter les vraies fonctions de manager

typedef DynamixelMotor* MotorGeneratorFunctionType(uint8_t, DynamixelPacketSender*);
//!High-level DynamixelMotor interface
/*!
 * The first goal of the DynamixelManager is to provide a way for every DynamixelMotor to communicate.
 * It provides the sendPacket() function which sends a DynamixelPacket and returns the response.
 * <br>The second goal of the DynamixelManager is to provide a high-level interface to use DynamixelMotor objects :
 * \li Motor instantiation and ID conflict prevention
 * \li Motor control by ID
 */
class DynamixelManager: public DynamixelPacketSender {

public:

    /**
     * Constructs a new DynamixelManager with the serial used for communication with Dynamixel motors and one for debugging that must have begun communication (with begin() ) (can be left to NULL if not needed)
     */
    explicit DynamixelManager(HardwareSerial*, usb_serial_class*  = nullptr, long baudrate = 57600);


    /*!
     * Sends DynamixelPacket through serial port and returns response string if there should be one.
     * \warning If serial RX is not connected or DynamixelPacket::responseSize is too big, serial might take
     * some time - up to several seconds - before timing out and returning unpredictable data.
     * @param packet
     * @return Response string, eventually empty.
     */
    char* sendPacket(DynamixelPacketData *) const override;

    /*!
     * Reads a single DynamixelPacket from the serial port.
     * @param responseSize the expected packet size
     * @return The packet string.
     */
    char* readPacket(uint8_t responseSize) const override;

    /*!
     * Creates a motor instance based on the given function, registered with the given ID
     * @return a new motor instance
     */
    DynamixelMotor* createMotor(uint8_t, MotorGeneratorFunctionType);

    /*!
     * Get a motor instance based on the given ID
     */
     DynamixelMotor* getMotor(uint8_t);

    HardwareSerial* serial;
private:

    std::map<uint8_t, DynamixelMotor*> motorMap;

    usb_serial_class* debugSerial;
};

#endif //DYN_MANAGER_H
