#pragma once

#include "Arduino.h"

namespace EncoderTool
{
    template <typename register_t>
    class pin_t
    {
     public:
        pin_t(uint_fast8_t pin)
            : portInputReg(portInputRegister(pin)),
              portSetReg(portSetRegister(pin)),
              portClrReg(portClearRegister(pin)),
              pinMask(digitalPinToBitMask(pin))
        {}

        void set() const { *portSetReg |= pinMask; }
        void clear() const {*portClrReg |= pinMask;}
        uint_fast8_t get() const { return (*portInputReg & pinMask) != 0; }

     protected:
        volatile register_t* const portInputReg;
        volatile register_t* const portSetReg;
        volatile register_t* const portClrReg;
        const register_t pinMask;
    };
}