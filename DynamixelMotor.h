//
// Created by trotfunky on 07/09/18.
//

#ifndef DYNAMIXEL_MOTOR_H
#define DYNAMIXEL_MOTOR_H

#include "Arduino.h"
#include "DynamixelUtils.h"
#include "DynamixelManager.h"
#include <string>

class DynamixelMotor {

public:

    DynamixelMotor(uint8_t,DynamixelMotorData,const DynamixelManager &);

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

    virtual DynamixelPacket* makeWritePacket(DynamixelAccessData, unsigned char*) = 0;
    virtual DynamixelPacket* makeReadPacket(DynamixelAccessData) = 0;
    virtual bool decapsulatePacket(const std::string&) = 0;
    virtual bool decapsulatePacket(const std::string&, float&) = 0;

protected:

    const DynamixelManager& manager;

    uint8_t motorID;

    DynamixelMotorData motorData;
};


#endif //DYNAMIXELMOTOR_H
