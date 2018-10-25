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
    bool decapsulatePacket(std::string) override;
    bool decapsulatePacket(std::string, float&) override;
};


#endif //XL30_XL430_H
