//
// Created by trotfunky on 07/09/18.
//

#include "DynamixelMotor.h"

DynamixelMotor::DynamixelMotor(uint8_t id, DynamixelMotorData data, const DynamixelManager& dynamixelManager) : manager(dynamixelManager),motorID(id), motorData(data)
{

}

int DynamixelMotor::getId() {
    return motorID;
}

float DynamixelMotor::getTorqueFromValue()
{
    return(motorData.valueToTorque);
}

float DynamixelMotor::getAngleFromValue()
{
    return(motorData.valueToAngle);
}

float DynamixelMotor::getVelocityFromValue()
{
    return(motorData.valueToVelocity);
}


/*
 *
 * These functions are called by the DynamixelManager in order to perform specific actions
 *
 */


bool DynamixelMotor::changeID(uint8_t id)
{
    char parameter[1] = {id};
    motorID = id;
    char* returnPacket = manager.sendPacket(makeWritePacket(motorData.id, parameter));
    return(decapsulatePacket(returnPacket));
}

bool DynamixelMotor::changeLED(bool state)
{
    char parameter[1] = {state};
    char* returnPacket = manager.sendPacket(makeWritePacket(motorData.led,parameter));
    return(decapsulatePacket(returnPacket));
}

bool DynamixelMotor::toggleTorque(bool state)
{
    char parameter[1] = {state};
    char* returnPacket = manager.sendPacket(makeWritePacket(motorData.torqueEnable,parameter));
    return(decapsulatePacket(returnPacket));
}

bool DynamixelMotor::setGoalAngle(float targetAngleDegree)
{
    uint32_t targetAngleValue = (uint32_t)(targetAngleDegree/motorData.valueToAngle);
    char parameter[motorData.goalAngle.length];

    for(int i = 0;i<motorData.goalAngle.length;i++)
    {
        parameter[i] = targetAngleValue & 0xFF;
        targetAngleValue = targetAngleValue >> 8;
    }

    char* returnPacket = manager.sendPacket(makeWritePacket(motorData.goalAngle,parameter));
    return(decapsulatePacket(returnPacket));
}

bool DynamixelMotor::getCurrentAngle(float &angle)
{
    char* returnPacket = manager.sendPacket(makeReadPacket(motorData.currentAngle));
    bool status = decapsulatePacket(returnPacket,angle);
    angle /= 2909;

    return(status);
}

bool DynamixelMotor::setGoalVelocity(float targetVelocity)
{
    uint32_t targetVelocityValue = (uint32_t)(targetVelocity/motorData.valueToVelocity);
    char parameter[motorData.goalVelocity.length];

    for(int i = 0;i<motorData.goalVelocity.length;i++)
    {
        parameter[i] = targetVelocityValue & 0xFF;
        targetVelocityValue = targetVelocityValue >> 8;
    }
    char* returnPacket = manager.sendPacket(makeWritePacket(motorData.goalVelocity,parameter));
    return(decapsulatePacket(returnPacket));
}

bool DynamixelMotor::getCurrentVelocity(float &velocity)
{
    char* returnPacket = manager.sendPacket(makeReadPacket(motorData.currentVelocity));
    bool status = decapsulatePacket(returnPacket,velocity);
    velocity *= getVelocityFromValue();

    return(status);
}

bool DynamixelMotor::getCurrentTorque(float &torque)
{
    char* returnPacket = manager.sendPacket(makeReadPacket(motorData.currentTorque));
    bool status = decapsulatePacket(returnPacket,torque);
    torque *= getTorqueFromValue();

    return(status);
}
