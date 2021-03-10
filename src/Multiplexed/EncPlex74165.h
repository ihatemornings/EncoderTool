#pragma once

#include "../delay.h"
#include "Bounce2.h"
#include "EncPlexBase.h"
#include "Arduino.h"
#include <initializer_list>

namespace EncoderTool
{
    class EncPlex74165 : public EncPlexBase
    {
     public:
        inline EncPlex74165(unsigned nrOfEncoders, unsigned pinLD, unsigned pinCLK, unsigned pinA, unsigned pinB, unsigned pinBtn = UINT32_MAX);
        inline ~EncPlex74165();

        inline void begin(CountMode mode = CountMode::quarter);
        inline void begin(allCallback_t callback, CountMode m = CountMode::quarter);
        inline void tick(); // call as often as possible

     protected:
        const unsigned A, B, Btn, LD, CLK;
    };

    // IMPLEMENTATION ============================================

    EncPlex74165::EncPlex74165(unsigned nrOfEncoders, unsigned pinLD, unsigned pinCLK, unsigned pinA, unsigned pinB, unsigned pinBtn)
        : EncPlexBase(nrOfEncoders), A(pinA), B(pinB), Btn(pinBtn), LD(pinLD), CLK(pinCLK)
    {
    }

    EncPlex74165::~EncPlex74165()
    {
        pinMode(A, INPUT);
        pinMode(B, INPUT);
        pinMode(Btn, INPUT);
        pinMode(LD, INPUT);
        pinMode(CLK, INPUT);
        
        // for (unsigned pin : {A, B, Btn, LD, CLK})
        // {
        //     pinMode(pin, INPUT);
        // };
    }

    void EncPlex74165::begin(allCallback_t cb, CountMode mode)
    {
        begin(mode);
        attachCallback(cb);
    }

    void EncPlex74165::begin(CountMode mode)
    {
        EncPlexBase::begin(mode);

        for (uint8_t pin : {A, B, Btn}) { pinMode(pin, INPUT); }
        for (uint8_t pin : {LD, CLK}) { pinMode(pin, OUTPUT); }

        d_write(LD, HIGH); // active low
        delayMicroseconds(1);

        // load current values to shift register
        d_write(LD, LOW);
        delay50ns();
        delay50ns();
        delay50ns();
        d_write(LD, HIGH);

        // first values are available directly after loading
        encoders[0].begin(d_read(A), d_read(B));

        for (unsigned i = 1; i < encoderCount; i++) // shift in the the rest of the encoders
        {
            d_write(CLK, HIGH);
            delay50ns();
            encoders[i].begin(d_read(A), d_read(B));
            d_write(CLK, LOW);
            delay50ns();
        }
    }

    void EncPlex74165::tick()
    {
        // load current values to shift register
        d_write(LD, LOW);
        delay50ns();
        delay50ns();
        delay50ns();
        d_write(LD, HIGH);

        // first values are available directly after loading

        int a = d_read(A);
        int b = d_read(B);
        int btn = Btn != UINT32_MAX ? d_read(Btn) : LOW;

        int delta = encoders[0].update(a, b, btn);

        if (delta != 0 && callback != nullptr)
        {
            callback(0, encoders[0].getValue(), delta);
        }
        for (unsigned i = 1; i < encoderCount; i++) // shift in the the rest of the encoders
        {
            d_write(CLK, HIGH);
            delay50ns();
            int delta = encoders[i].update(d_read(A), d_read(B), Btn != UINT32_MAX ? d_read(Btn) : LOW);
            if (delta != 0 && callback != nullptr)
            {
                callback(i, encoders[i].getValue(), delta);
            }
            d_write(CLK, LOW);
            delay50ns();
        }
    }

} // namespace EncoderTool