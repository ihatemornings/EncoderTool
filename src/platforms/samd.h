#pragma once

#include "Arduino.h"
namespace EncoderTool
{
    template <typename register_t>
    class pin_t
    {
     public:
        pin_t(uint_fast8_t pin)
            : port(digitalPinToPort(pin)),
              portInputReg(&(port->IN.reg)),
              portSetReg(&(port->OUTSET.reg)),
              portClrReg(&(port->OUTCLR.reg)),
              pinMask(digitalPinToBitMask(pin))
        {

        }

        void set() const { *portSetReg |= pinMask; }
        void clear() const { *portClrReg |= pinMask; }
        uint_fast8_t get() const { return (*portInputReg & pinMask) != 0 ; }

     protected:
        PortGroup* const port;
        const volatile register_t* const portInputReg;
        volatile register_t* const portSetReg;
        volatile register_t* const portClrReg;
        const register_t pinMask;
    };
}









#include "Arduino.h"

inline void d_write(uint32_t pin, uint32_t val)
{
  digitalWrite(pin, val);
}

inline int d_read(uint32_t pin)
{
  return digitalRead(pin);
}

#if defined(__SAMD51__)

#define PLAIN_ENC_CALLBACK

#else


#endif