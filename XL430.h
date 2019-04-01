//
// Created by trotfunky on 10/09/18.
//

#ifndef XL30_XL430_H
#define XL30_XL430_H

#include "DynamixelMotor.h"

enum XL430OperatingModes {
    VELOCITY_CONTROL_MODE = 1,
    POSITION_CONTROL_MODE = 3,
    EXTENDED_POSITION_CONTROL_MODE = 4,
    PWN_CONTROL_MODE = 16,
};

//! XL430-specific class
/*!
 * \sa XL430 documentation : http://emanual.robotis.com/docs/en/dxl/x/xl430-w250/
 */
class XL430 : public DynamixelMotor
{
public:
    explicit XL430(uint8_t,const DynamixelPacketSender &);

    DynamixelPacketData* makeWritePacket(DynamixelAccessData, char *) override;
    DynamixelPacketData* makeReadPacket(DynamixelAccessData) override;
    bool decapsulatePacket(const char *) override;
    bool decapsulatePacket(const char *, float &) override;
    bool decapsulatePacket(const char *, int &) override;

    //! The static members are used in order to minimize the memory usage of each individual object.
    static const DynamixelAccessData& xl430GoalAngle;
    static const DynamixelAccessData& xl430ID;
    static const DynamixelAccessData& xl430LED;
    static const DynamixelAccessData& xl430TorqueEnable;
    static const DynamixelAccessData& xl430CurrentTorque;
    static const DynamixelAccessData& xl430CurrentAngle;
    static const DynamixelAccessData& xl430GoalVelocity;
    static const DynamixelAccessData& xl430CurrentVelocity;
    static const DynamixelAccessData& xl430OperatingMode;
    static const DynamixelAccessData& xl430VelocityLimit;
    static const DynamixelAccessData& xl430Moving;
    static const DynamixelAccessData& xl430ReturnDelay;

private:

    static constexpr float torqueConversionFactor = 1/1024.0f;
    static constexpr float angleConversionFactor = 0.088;
    static constexpr float velocityConversionFactor = 0.229;
};

DynamixelMotor* XL430GeneratorFunction(uint8_t id, DynamixelPacketSender* packetSender);


#endif //XL30_XL430_H
