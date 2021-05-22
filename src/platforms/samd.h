#pragma once

#define PLAIN_ENC_CALLBACK // uncomment if you prefer plain vanilla callbacks instead of std::function

#include "WInterrupts.h"
#include "delay.h"
#include "pins_arduino.h"
#include "wiring_digital.h"

namespace EncoderTool
{
    class Pin
    {
     public:
        Pin(uint_fast8_t pin)
            : port(digitalPinToPort(pin)),
              portInputReg(port->IN.reg),
              portSetReg(port->OUTSET.reg),
              portClrReg(port->OUTCLR.reg),
              pinMask(digitalPinToBitMask(pin))
        {}

        void set() const { portSetReg = pinMask; }
        void clear() const { portClrReg = pinMask; }
        uint_fast8_t get() const { return (portInputReg & pinMask) != 0; }

     protected:
        PortGroup* const port;
        const volatile uint32_t& portInputReg;
        volatile uint32_t& portSetReg;
        volatile uint32_t& portClrReg;
        const uint32_t pinMask;
    };

    __attribute__((always_inline))
    inline void delay50ns()
    {
    }

    __attribute__((always_inline))
    inline void delay200ns()
    {
        __asm__ volatile("nop\n");
        __asm__ volatile("nop\n");

        return;
    }

}

inline void d_write(uint32_t pin, uint32_t val)
{
    digitalWrite(pin, val);
}

inline int d_read(uint32_t pin)
{
    return digitalRead(pin);
}
