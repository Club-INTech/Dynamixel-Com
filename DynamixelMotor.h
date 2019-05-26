//
// Created by trotfunky on 07/09/18.
//

#ifndef DYNAMIXEL_MOTOR_H
#define DYNAMIXEL_MOTOR_H

#include "Arduino.h"
#include "DynamixelUtils.h"
#include "DynamixelPacketSender.h"


//! Abstract class for Dynamixel Motors
/*!
 * This class defines basic functions that every kind of motor can use without having to override them. In order for this
 * to work the makeWritePacket(), makeReadPacket() and decapsulatePacket() have to be defined, allowing for multiple protocols
 * using the same abstract class.
 */
class DynamixelMotor {

public:

    DynamixelMotor(uint8_t,DynamixelMotorData,const DynamixelPacketSender &);



    /*!
     * \name Pre-defined functions
     * These functions do not usually need to be overridden and can be used as is, provided the pure virtual functions
     * are defined.
     * \warning These functions are defined using little endian.
     */
    //!@{
    virtual int getId();

    virtual float getTorqueFromValue();
    virtual float getAngleFromValue();
    virtual float getVelocityFromValue();

    virtual bool changeID(uint8_t);
    virtual bool changeLED(bool);
    virtual bool toggleTorque(bool);
    virtual bool setGoalAngle(float);
    virtual bool getCurrentAngle(float&);
    virtual bool setGoalVelocity(float);
    virtual bool getCurrentVelocity(float&);
    virtual bool getCurrentTorque(int&);
    virtual bool getOperatingMode(uint8_t&);
    virtual bool setOperatingMode(uint8_t);
    virtual void reboot() = 0;
    virtual bool checkAlert(const char *packet) = 0;
    //!@}



    /*!
     * \name Functions to override
     * These functions *have* to be properly defined otherwise the Pre-defined functions will not work.
     * \sa XL430 for implementation example.
     */
    //!@{
    virtual DynamixelPacketData* makeWritePacket(DynamixelAccessData, char *) = 0;
    virtual DynamixelPacketData* makeReadPacket(DynamixelAccessData) = 0;


    //! Checks status packet
    /*!
     * Checks error-detection codes and packet status
     * @return false if there is any error, true otherwise.
     */
    virtual bool decapsulatePacket(const char *) = 0;

    //! Checks status packet and extracts floating point data
    /*!
     * Calls decapsulatePacket(const std::string&) for error checking and extracts data if everything is right.
     * @return false if there is any error, true otherwise.
     */
    virtual bool decapsulatePacket(const char *, float &) = 0;

    virtual bool decapsulatePacket(const char *, int &) = 0;
    //!@}

protected:

    const DynamixelPacketSender& manager;

    uint8_t motorID;

    DynamixelMotorData motorData;
};


#endif //DYNAMIXELMOTOR_H
