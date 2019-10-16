//
// Created by trotfunky on 07/09/18.
//

#ifndef DYN_MANAGER_H
#define DYN_MANAGER_H

#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DynamixelUtils.h"
#include "DynamixelPacketSender.h"
#include "DynamixelMotor.h"
#include <map>

// TODO : Rajouter les vraies fonctions de manager

typedef DynamixelMotor* MotorGeneratorFunctionType(uint8_t, DynamixelPacketSender*);
//!High-level DynamixelMotor interface
/*!
 * The first goal of the DynamixelManager is to provide a way for every DynamixelMotor to communicate.
 * It provides the sendPacket() function which sends a DynamixelPacket and returns the response.
 * <br>The second goal of the DynamixelManager is to provide a high-level interface to use DynamixelMotor objects :
 * \li Motor instantiation and ID conflict prevention
 * \li Motor control by ID
 */
class DynamixelManager: public DynamixelPacketSender {

public:
    /**
     * Dumb constructor, common to the other ones.
     * @param debugSerial Serial to output debug messages.
     */
    explicit DynamixelManager(Stream* debugSerial);
    /**
     * Constructs a new DynamixelManager with the serial used for communication with Dynamixel motors and one for debugging that must have begun communication (with begin() ) (can be left to NULL if not needed)
     */
    explicit DynamixelManager(HardwareSerial*, Stream*  = nullptr, long baudrate = 57600);
    /**
     * Constructs a new DynamixelManager and a new SoftwareSerial on the designated pins.
     * @param RX Receive pin, board side
     * @param TX Transmit pin, board side
     * @param baudrate
     */
    DynamixelManager(int RX, int TX, Stream*  = nullptr, long baudrate = 57600);


    /*!
     * Sends DynamixelPacket through serial port and returns response string if there should be one.
     * \warning If serial RX is not connected or DynamixelPacket::responseSize is too big, serial might take
     * some time - up to several seconds - before timing out and returning unpredictable data.
     * @param packet
     * @return Response string, eventually empty.
     */
    char* sendPacket(DynamixelPacketData *) const override;

    /*!
     * Reads a single DynamixelPacket from the serial port.
     * @param responseSize the expected packet size
     * @return The packet string.
     */
    char* readPacket(uint8_t responseSize) const override;

    /*!
     * Creates a motor instance based on the given function, registered with the given ID
     * @return a new motor instance
     */
    DynamixelMotor* createMotor(uint8_t, MotorGeneratorFunctionType);

    /*!
     * Get a motor instance based on the given ID
     */
     DynamixelMotor* getMotor(uint8_t);

private:
    Stream* serial;

    std::map<uint8_t, DynamixelMotor*> motorMap;

    Stream* debugSerial;

    int TX;
    int RX;

    void setHalfDuplex(const Stream & mStream) const
    {
#if defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__) // Teensy 3.0 3.1 3.2 3.5 3.6
        if (&mStream == &Serial1)
        {
            UART0_C1 |= UART_C1_LOOPS | UART_C1_RSRC; // Connect internally RX and TX for half duplex
            CORE_PIN1_CONFIG |= PORT_PCR_PE | PORT_PCR_PS; // pullup on output pin
        }
        else if (&mStream == &Serial2)
        {
            UART1_C1 |= UART_C1_LOOPS | UART_C1_RSRC; // Connect internally RX and TX for half duplex
            CORE_PIN10_CONFIG |= PORT_PCR_PE | PORT_PCR_PS; // pullup on output pin
        }
        else if (&mStream == &Serial3)
        {
            UART2_C1 |= UART_C1_LOOPS | UART_C1_RSRC; // Connect internally RX and TX for half duplex
            CORE_PIN8_CONFIG |= PORT_PCR_PE | PORT_PCR_PS; // pullup on output pin
        }
#if defined(__MK64FX512__) || defined(__MK66FX1M0__) // Teensy 3.5 or 3.6
        else if (&mStream == &Serial4)
        {
            UART3_C1 |= UART_C1_LOOPS | UART_C1_RSRC; // Connect internally RX and TX for half duplex
            CORE_PIN32_CONFIG |= PORT_PCR_PE | PORT_PCR_PS; // pullup on output pin
        }
        else if (&mStream == &Serial5)
        {
            UART4_C1 |= UART_C1_LOOPS | UART_C1_RSRC; // Connect internally RX and TX for half duplex
            CORE_PIN33_CONFIG |= PORT_PCR_PE | PORT_PCR_PS; // pullup on output pin
        }
        else if (&mStream == &Serial6)
        {
            UART5_C1 |= UART_C1_LOOPS | UART_C1_RSRC; // Connect internally RX and TX for half duplex
            CORE_PIN48_CONFIG |= PORT_PCR_PE | PORT_PCR_PS; // pullup on output pin
        }
#endif
#endif
    }

    void setReadMode(const Stream & mStream) const
    {
#if defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__) // Teensy 3.0 3.1 3.2 3.5 3.6
        if (&mStream == &Serial1)
        {
            UART0_C3 &= ~UART_C3_TXDIR;
        }
        else if (&mStream == &Serial2)
        {
            UART1_C3 &= ~UART_C3_TXDIR;
        }
        else if (&mStream == &Serial3)
        {
            UART2_C3 &= ~UART_C3_TXDIR;
        }
#if defined(__MK64FX512__) || defined(__MK66FX1M0__) // Teensy 3.5 or 3.6
        else if (&mStream == &Serial4)
        {
            UART3_C3 &= ~UART_C3_TXDIR;
        }
        else if (&mStream == &Serial5)
        {
            UART4_C3 &= ~UART_C3_TXDIR;
        }
        else if (&mStream == &Serial6)
        {
            UART5_C3 &= ~UART_C3_TXDIR;
        }
#endif
#else
        pinMode(TX,INPUT);
#endif
    }


    void setWriteMode(const Stream & mStream) const
    {
#if defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__) // Teensy 3.0 3.1 3.2 3.5 3.6
        if (&mStream == &Serial1)
        {
            UART0_C3 |= UART_C3_TXDIR;
        }
        else if (&mStream == &Serial2)
        {
            UART1_C3 |= UART_C3_TXDIR;
        }
        else if (&mStream == &Serial3)
        {
            UART2_C3 |= UART_C3_TXDIR;
        }
#if defined(__MK64FX512__) || defined(__MK66FX1M0__) // Teensy 3.5 ou 3.6
        else if (&mStream == &Serial4)
        {
            UART3_C3 |= UART_C3_TXDIR;
        }
        else if (&mStream == &Serial5)
        {
            UART4_C3 |= UART_C3_TXDIR;
        }
        else if (&mStream == &Serial6)
        {
            UART5_C3 |= UART_C3_TXDIR;
        }
#endif
#else
        pinMode(TX,OUTPUT);
#endif
    }

};

#endif //DYN_MANAGER_H
