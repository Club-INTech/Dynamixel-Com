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

//! XL430 specific class
/*!
 * \sa XL430 documentation : http://emanual.robotis.com/docs/en/dxl/x/xl430-w250/
 */
class XL430 : public DynamixelMotor
{
public:
    XL430(uint8_t,const DynamixelPacketSender &);

    DynamixelPacketData* makeWritePacket(DynamixelAccessData, char *) override;
    DynamixelPacketData* makeReadPacket(DynamixelAccessData) override;
    bool decapsulatePacket(const char *) override;
    bool decapsulatePacket(const char *, float &) override;
    bool decapsulatePacket(const char *, int &) override;
    bool checkAlert(const char *packet) override;
    void reboot() override;

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
    static const DynamixelAccessData& moving;
    static const DynamixelAccessData& movingStatus;
    static const DynamixelAccessData& returnDelay;
    static const DynamixelAccessData& hardwareError;
    static const DynamixelAccessData& movingThreshold;
    static const DynamixelAccessData& movingOffset;
    static const DynamixelAccessData& watchdog;

private:

    static constexpr float torqueConversionFactor = 1/1024.0f;
    static constexpr float angleConversionFactor = 0.088;
    static constexpr float velocityConversionFactor = 0.229;
};

DynamixelMotor* XL430GeneratorFunction(uint8_t id, DynamixelPacketSender* packetSender);


#endif //XL30_XL430_H
