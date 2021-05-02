#pragma once

#include "../EncoderBase.h"
#include "../config.h"
#include "attachInterruptEx.h"

namespace EncoderTool
{
    // Simple encoder implementation which reads phase A and B from two digital pins
    // It uses attachInterruptEx to attach callbacks with type void(*)(Encoder*).
    // We don't use std::function<> here to reduce memory footprint for small MCUs like the T-LC

    class Encoder : public EncoderBase
    {
     public:
        Encoder() = default;
        inline ~Encoder();

        inline void begin(uint8_t pinA, uint8_t pinB, CountMode = CountMode::quarter, uint8_t inputMode = INPUT_PULLUP);
        inline void begin(uint8_t pinA, uint8_t pinB, encCallback_t cb, CountMode = CountMode::quarter, uint8_t inputMode = INPUT_PULLUP);

     protected:
        uint8_t pinNrA, pinNrB;
        Pin* A, * B;
    };

    // Inline implementation ===============================================

    void Encoder::begin(uint8_t pinA, uint8_t pinB, encCallback_t cb, CountMode countMode, uint8_t inputMode)
    {
        begin(pinA, pinB, countMode, inputMode);
        attachCallback(cb);
    }

    void Encoder::begin(uint8_t _pinA, uint8_t _pinB, CountMode countMode, uint8_t inputMode)
    {
        pinNrA = _pinA;
        pinNrB = _pinB;
        pinMode(_pinA, inputMode);
        pinMode(_pinB, inputMode);
        A = new Pin(_pinA);
        B = new Pin(_pinB);

        attachInterruptEx(pinNrA, [](Encoder* THIS) { THIS->update(THIS->A->get(), THIS->B->get()); }, this, CHANGE);
        attachInterruptEx(pinNrB, [](Encoder* THIS) { THIS->update(THIS->A->get(), THIS->B->get()); }, this, CHANGE);

        setCountMode(countMode);
        EncoderBase::begin(A->get(), B->get()); // set start state
    }

    Encoder::~Encoder()
    {
        detachInterrupt(pinNrA);
        detachInterrupt(pinNrB);
        delete A;
        delete B;
    }
} // namespace EncoderTool