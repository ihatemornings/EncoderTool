#include "attachInterruptEx.h"
#include "Arduino.h"

namespace EncoderTool
{
    constexpr unsigned N = EXTERNAL_NUM_INTERRUPTS;

    namespace // private
    {
        using relay_t = void (*)();

        callback_t callbacks[N]; // pointers to callback functions
        Encoder* states[N];      // state passed to callback functions
        relay_t relays[N];       // pointers to functions relaying calls from pin interrupts to the callback functions
        uint8_t pins[N];         // keep track which pin is attached to which callback

        // intialize the array of relay functions recursively
        template <size_t N>
        void initRelays()
        {
            pins[N]   = UINT8_MAX;
            relays[N] = [] { callbacks[N](states[N]); }; // lambda expression relaying the pin ISR to the actual callback function
            initRelays<N - 1>();
        }

        template <>
        void initRelays<0>()
        {
            pins[0]   = UINT8_MAX;
            relays[0] = [] { callbacks[0](states[0]); };
        }

        bool initialized = false;
        void initialize()
        {
            initRelays<N - 1>();
            initialized = true;
        }

    } // private namespace

    relay_t getFunc(callback_t callback, Encoder* state)
    {
        if (!initialized) initialize();

        for (unsigned i = 0; i < N; i++)
        {
            if (callbacks[i] == nullptr)
            {
                callbacks[i] = callback;
                states[i]    = state;
                return relays[i];
            }
        }
        return nullptr;
    }

    void releaseFunc(relay_t r)
    {
        for (unsigned i = 0; i < N; i++)
        {
            if (relays[i] == r)
            {
                callbacks[i] = nullptr;
                return;
            }
        }
    }

    bool attachInterruptEx(unsigned pin, callback_t callback, Encoder* state, int mode)
    {
        auto r = getFunc([](Encoder*) { Serial.println("asdf"); }, nullptr);


        if (!initialized) initialize();

        for (unsigned i = 0; i < N; i++)
        {
            if (pins[i] == UINT8_MAX || pins[i] == pin)
            {
                pins[i]      = pin;
                callbacks[i] = callback;
                states[i]    = state;
                attachInterrupt(pin, relays[i], mode);
                return true;
            }
        }
        return false;
    }

    bool detachInterruptEx(unsigned pin)
    {
        detachInterrupt(pin);

        for (unsigned i = 0; i < N; i++)
        {
            if (pins[i] == pin)
            {
                pins[i] = UINT8_MAX;
                return true;
            }
        }
        return false;
    }
}