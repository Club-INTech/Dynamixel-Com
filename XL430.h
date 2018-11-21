//
// Created by trotfunky on 10/09/18.
//

#ifndef XL30_XL430_H
#define XL30_XL430_H

#include "DynamixelMotor.h"


//! XL430-specific class
/*!
 * \sa XL430 documentation : http://emanual.robotis.com/docs/en/dxl/x/xl430-w250/
 */
class XL430 : public DynamixelMotor
{
public:
    explicit XL430(uint8_t,const DynamixelManager &);

    DynamixelPacketData* makeWritePacket(DynamixelAccessData, char *) override;
    DynamixelPacketData* makeReadPacket(DynamixelAccessData) override;
    bool decapsulatePacket(const char *) override;
    bool decapsulatePacket(const char *, float &) override;

    //! The static members are used in order to minimize the memory usage of each individual object.
    static const DynamixelAccessData& xl430GoalAngle;
    static const DynamixelAccessData& xl430ID;
    static const DynamixelAccessData& xl430LED;
    static const DynamixelAccessData& xl430TorqueEnable;
    static const DynamixelAccessData& xl430CurrentTorque;
    static const DynamixelAccessData& xl430CurrentAngle;
    static const DynamixelAccessData& xl430GoalVelocity;
    static const DynamixelAccessData& xl430CurrentVelocity;

private:

    static constexpr float torqueConversionFactor = 0.0015;
    static constexpr float angleConversionFactor = 0.088;
    static constexpr float velocityConversionFactor = 0.229;
};


#endif //XL30_XL430_H
