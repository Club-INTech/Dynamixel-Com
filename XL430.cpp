//
// Created by trotfunky on 10/09/18.
//

#include "XL430.h"

XL430::XL430(uint8_t id, const DynamixelManager& dynamixelManager) : DynamixelMotor(id,
                                          DynamixelMotorData(id,DynamixelAccessData(0x07,0x00,1),
                                          DynamixelAccessData(0x41,0x00,1), DynamixelAccessData(0x40,0x00,1),
                                          DynamixelAccessData(0x7e,0x00,2), DynamixelAccessData(0x74,0x00,4),
                                          DynamixelAccessData(0x84,0x00,4), DynamixelAccessData(0x68,0x00,4),
                                          DynamixelAccessData(0x80,0x00,4), 0.0015, 0.088,0.229), dynamixelManager)
{

}

DynamixelPacket* XL430::makeWritePacket(DynamixelAccessData accessData, unsigned char * parameters)
{
    uint8_t packetSize = 4+1+2+1+2+accessData.length+2;
    unsigned char* packet = new unsigned char[packetSize];

    int position = 0;
    for(int i=0;i<4;i++)
    {
        packet[position] = v2Header[i];
        position ++;
    }

    packet[position] = motorData.motorID;
    motorData.motorID = motorID;
    position ++;
    int instructionsLength = 1+2+accessData.length+2;
    packet[position] = instructionsLength & 0xFF;
    position++;
    packet[position] = (instructionsLength >> 8) & 0xFF;
    position++;
    packet[position] = 0x03;
    position++;
    packet[position] = accessData.address[0];
    position++;
    packet[position] = accessData.address[1];
    position++;

    for(int i = 0;i<accessData.length;i++)
    {
        packet[position] = parameters[i];
        position++;
    }

//    Serial.println(instructionsLength+5);

    unsigned short crc = crc_compute(packet,instructionsLength+5);
    packet[position] = crc & 0xFF;
    position++;
    packet[position] = (crc >> 8) & 0xFF;

//    Serial.println(packetSize);
//    Serial.println("Sent: ");
//    for(int i = 0;i<packetSize;i++)
//    {
//        Serial.print(*(packet+i));
//        Serial.print(",");
//    }
//    Serial.println("");

    return(new DynamixelPacket(packet,packetSize,11));
}

DynamixelPacket* XL430::makeReadPacket(DynamixelAccessData)
{

}

bool XL430::decapsulatePacket(const std::string &packet)
{
    unsigned short responseLength = 7 + packet[5] + (packet[6] << 8) - 2;

    if(crc_compute((unsigned char*)packet.c_str(),responseLength) == (packet[responseLength]+(packet[responseLength+1] << 8)))
    {
        if((unsigned int)packet[8] < 128 && (int)packet[7] == 0x55)
        {
            return(true);
        }
    }
    return(false);
}

bool XL430::decapsulatePacket(const std::string& packet, float &value) {
    if(decapsulatePacket(packet))
    {
        int parameterLength = packet[5] + (packet[6] << 8) - 4;

        for(int i = 0; i<parameterLength; i++)
        {
            value += (int)(packet[9+i] << 8*i);
        }

        return(true);
    }
    else
    {
        value = 0;
        return(false);
    }
}
