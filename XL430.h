//
// Created by trotfunky on 10/09/18.
//

#ifndef XL30_XL430_H
#define XL30_XL430_H

#include "DynamixelMotor.h"

class XL430 : public DynamixelMotor
{
public:
    explicit XL430(uint8_t,const DynamixelManager &);

    DynamixelPacket* makeWritePacket(DynamixelAccessData, unsigned char*) override;
    DynamixelPacket* makeReadPacket(DynamixelAccessData) override;
    bool decapsulatePacket(const std::string&) override;
    bool decapsulatePacket(const std::string&, float&) override;

private:

    static const DynamixelAccessData& xl430ID;
    static const DynamixelAccessData& xl430LED;
    static const DynamixelAccessData& xl430TorqueEnable;
    static const DynamixelAccessData& xl430CurrentTorque;
    static const DynamixelAccessData& xl430GoalAngle;
    static const DynamixelAccessData& xl430CurrentAngle;
    static const DynamixelAccessData& xl430GoalVelocity;
    static const DynamixelAccessData& xl430CurrentVelocity;

    static constexpr float torqueConversionFactor = 0.0015;
    static constexpr float angleConversionFactor = 0.088;
    static constexpr float velocityConversionFactor = 0.229;
};


#endif //XL30_XL430_H
