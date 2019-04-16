//
// Created by trotfunky on 16/04/19.
//

#ifndef XL30_AX12_H
#define XL30_AX12_H

#include "DynamixelMotor.h"
#include "DynamixelUtils.h"
#include "DynamixelPacketSender.h"

class AX12 : DynamixelMotor
{
public:

    AX12(uint8_t,const DynamixelPacketSender &);

    DynamixelPacketData* makeWritePacket(DynamixelAccessData, char *) override;
    DynamixelPacketData* makeReadPacket(DynamixelAccessData) override;
    // TODO Implement
    bool decapsulatePacket(const char *) override;
    bool decapsulatePacket(const char *, float &) override;
    bool decapsulatePacket(const char *, int &) override;

    //! The static members are used in order to minimize the memory usage of each individual object.
    static const DynamixelAccessData& ax12GoalAngle;
    static const DynamixelAccessData& ax12ID;
    static const DynamixelAccessData& ax12LED;
    static const DynamixelAccessData& ax12TorqueEnable;
    static const DynamixelAccessData& ax12CurrentTorque;
    static const DynamixelAccessData& ax12CurrentAngle;
    static const DynamixelAccessData& ax12GoalVelocity;
    static const DynamixelAccessData& ax12CurrentVelocity;
    static const DynamixelAccessData& ax12OperatingMode;

private:

    static constexpr float torqueConversionFactor = 1/1024.0f;
    static constexpr float angleConversionFactor = 0.088;
    static constexpr float velocityConversionFactor = 0.229;
};


#endif //XL30_AX12_H
