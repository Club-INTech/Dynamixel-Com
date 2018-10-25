//
// Created by trotfunky on 07/09/18.
//

#ifndef DYN_MANAGER_H
#define DYN_MANAGER_H

#include "Arduino.h"
#include "DynamixelUtils.h"
#include <string>

// TODO : Rajouter les vraies fonctions de manager

class DynamixelManager {

public:
    DynamixelManager(HardwareSerial*);

    std::string sendPacket(DynamixelPacket *packet) const;



private:

    HardwareSerial* serial;

};

#endif //DYN_MANAGER_H
