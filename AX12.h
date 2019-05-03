//
// Created by trotfunky on 16/04/19.
//

#ifndef XL30_AX12_H
#define XL30_AX12_H

#include "DynamixelMotor.h"
#include "DynamixelUtils.h"
#include "DynamixelPacketSender.h"

class AX12 : public DynamixelMotor
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
    static const DynamixelAccessData& goalAngle;
    static const DynamixelAccessData& ID;
    static const DynamixelAccessData& LED;
    static const DynamixelAccessData& torqueEnable;
    static const DynamixelAccessData& currentTorque;
    static const DynamixelAccessData& currentAngle;
    static const DynamixelAccessData& goalVelocity;
    static const DynamixelAccessData& currentVelocity;
    static const DynamixelAccessData& operatingMode;

    static const DynamixelAccessData& velocityLimit;
    static const DynamixelAccessData& returnDelay;
    static const DynamixelAccessData& moving;
    static const DynamixelAccessData& movingStatus;
    static const DynamixelAccessData& hardwareError;
    static const DynamixelAccessData& movingThreshold;

private:

    static constexpr float torqueConversionFactor = 1/1024.0f;
    static constexpr float angleConversionFactor = 0.088;
    static constexpr float velocityConversionFactor = 0.229;
};

DynamixelMotor* AX12GeneratorFunction(uint8_t id, DynamixelPacketSender* packetSender);

#endif //XL30_AX12_H
