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
};

const DynamixelAccessData& XL430::xl430ID = DynamixelAccessData(0x07,0x00,1);
const DynamixelAccessData& XL430::xl430LED = DynamixelAccessData(0x41,0x00,1);
const DynamixelAccessData& XL430::xl430TorqueEnable = DynamixelAccessData(0x40,0x00,1);
const DynamixelAccessData& XL430::xl430CurrentTorque = DynamixelAccessData(0x7e,0x00,2);
const DynamixelAccessData& XL430::xl430GoalAngle = DynamixelAccessData(0x74,0x00,4);
const DynamixelAccessData& XL430::xl430CurrentAngle = DynamixelAccessData(0x84,0x00,4);
const DynamixelAccessData& XL430::xl430GoalVelocity = DynamixelAccessData(0x68,0x00,4);
const DynamixelAccessData& XL430::xl430CurrentVelocity = DynamixelAccessData(0x80,0x00,4);

#endif //XL30_XL430_H
