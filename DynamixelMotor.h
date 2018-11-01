//
// Created by trotfunky on 07/09/18.
//

#ifndef DYNAMIXEL_MOTOR_H
#define DYNAMIXEL_MOTOR_H

#include "Arduino.h"
#include "DynamixelUtils.h"
#include "DynamixelManager.h"
#include <string>


//! Abstract class for Dynamixel Motors
/*!
 * This class defines basic functions that every kind of motor can use without having to override them. In order for this
 * to work the makeWritePacket(), makeReadPacket() and decapsulatePacket() have to be defined, allowing for multiple protocols
 * using the same abstract class.
 */
class DynamixelMotor {

public:

    DynamixelMotor(uint8_t,DynamixelMotorData,const DynamixelManager &);



    /*!
     * \name Defined functions
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
    virtual bool getCurrentTorque(float&);
    //!@}



    /*!
     * \name Functions to override
     * These functions *have* to be properly defined otherwise the defined functions will not work.
     * \sa XL430 for implementation example.
     */
    //!@{
    virtual DynamixelPacket* makeWritePacket(DynamixelAccessData, unsigned char*) = 0;
    virtual DynamixelPacket* makeReadPacket(DynamixelAccessData) = 0;


    //! Checks status packet
    /*!
     * Checks error-detection codes and packet status
     * @return false if there is any error, true otherwise.
     */
    virtual bool decapsulatePacket(const std::string&) = 0;

    //! Checks status packet and extracts data
    /*!
     * Calls decapsulatePacket(const std::string&) for error checking and extracts data if everything is right.
     * @return false if there is any error, true otherwise.
     */
    virtual bool decapsulatePacket(const std::string&, float&) = 0;
    //!@}

protected:

    const DynamixelManager& manager;

    uint8_t motorID;

    DynamixelMotorData motorData;
};


#endif //DYNAMIXELMOTOR_H
