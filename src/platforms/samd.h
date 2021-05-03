#pragma once

//#define PLAIN_ENC_CALLBACK  // comment out if you prefer plain vanilla callbacks instead of std::function

#if !defined(PLAIN_ENC_CALLBACK)
#include <functional>
#endif

#include "Arduino.h"
namespace EncoderTool
{
    class Pin
    {
     public:
        Pin(uint_fast8_t pin)
            : port(digitalPinToPort(pin)),
              portInputReg(&(port->IN.reg)),
              portSetReg(&(port->OUTSET.reg)),
              portClrReg(&(port->OUTCLR.reg)),
              pinMask(digitalPinToBitMask(pin))
        {}

        void set() const { *portSetReg |= pinMask; }
        void clear() const { *portClrReg |= pinMask; }
        uint_fast8_t get() const { return (*portInputReg & pinMask) != 0; }

     protected:
        PortGroup* const port;
        const volatile uint32_t* const portInputReg;
        volatile uint32_t* const portSetReg;
        volatile uint32_t* const portClrReg;
        const uint32_t pinMask;
    };
}



inline void d_write(uint32_t pin, uint32_t val)
{
    digitalWrite(pin, val);
}

inline int d_read(uint32_t pin)
{
    return digitalRead(pin);
}



