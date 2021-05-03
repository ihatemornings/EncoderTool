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
        Pin(uint_fast8_t _pin) : pin(_pin) {}
        void set() const { digitalWriteFast(pin, HIGH); }
        void clear() const { digitalWriteFast(pin, LOW); }
        uint_fast8_t get() const { return digitalRead(pin); }

     protected:
        const uint8_t pin;
    };
};

inline void d_write(uint32_t pin, uint32_t val)
{
    digitalWriteFast(pin, val);
}

inline int d_read(uint32_t pin)
{
    return digitalReadFast(pin);
}


