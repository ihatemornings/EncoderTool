#pragma once

#include "../EncoderBase.h"
#include "Arduino.h"
#include "Bounce2.h"

namespace EncoderTool
{
    // Simple encoder implementation which reads phase A and B from two digital pins
    class PolledEncoder : public EncoderBase
    {
     public:
        inline void begin(int pinA, int pinB, CountMode = CountMode::quarter, int inputMode = INPUT_PULLUP);
        inline void begin(int pinA, int pinB, int pinBtn, CountMode = CountMode::quarter, int inputMode = INPUT_PULLUP);

        inline void tick(); //call tick() as often as possible. For mechanical encoders a call frequency of > 5kHz should be sufficient

     protected:
        int pinA, pinB, pinBtn = UINT32_MAX;
        Pin *A, *B;
        Pin* pBtn = nullptr;
    };

    // Inline implementation ===============================================

    void PolledEncoder::tick()
    {
        update(A->get(), B->get(), pBtn != nullptr? pBtn->get() : LOW);
    }

    void PolledEncoder::begin(int pinA, int pinB, int pinBtn, CountMode countMode, int inputMode)
    {
        this->pinBtn = pinBtn;
        pinMode(pinBtn, inputMode);
        pBtn = new Pin(pinBtn);
        
        begin(pinA, pinB, countMode, inputMode);
    }

    void PolledEncoder::begin(int pinA, int pinB, CountMode countMode, int inputMode)
    {
        this->pinA = pinA;
        this->pinB = pinB;
        for (uint8_t pin : {pinA, pinB})
        {
            pinMode(pin, inputMode);
        }

        A = new Pin(pinA);
        B = new Pin(pinB);

        setCountMode(countMode);
        EncoderBase::begin(A->get(), B->get()); // set start state
    }
} // namespace EncoderTool