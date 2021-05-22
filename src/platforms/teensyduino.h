#pragma once

//#define PLAIN_ENC_CALLBACK  // comment out if you prefer plain vanilla callbacks instead of std::function


#include "Arduino.h"
namespace EncoderTool
{
    class Pin
    {
     public:
        Pin(uint_fast8_t _pin) : pin(_pin) {}
        void set() const { digitalWriteFast(pin, HIGH); }
        void clear() const { digitalWriteFast(pin, LOW); }
        uint_fast8_t get() const { return digitalRead(pin); }

     protected:
        const uint8_t pin;
    };

    inline void d_write(uint32_t pin, uint32_t val)
    {
        digitalWriteFast(pin, val);
    }

    inline int d_read(uint32_t pin)
    {
        return digitalReadFast(pin);
    }

#if defined(ARDUINO_TEENSY40) || defined(ARDUINO_TEENSY41) // delayNanoseconds will be available for all Teensies from TD 1.54 on

    inline void delay50ns()
    {
        delayNanoseconds(50);
    }

#else

    inline void delay50ns()
    {
        delayMicroseconds(1);
    }

#endif
};
