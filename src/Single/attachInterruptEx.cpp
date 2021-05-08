#include "attachInterruptEx.h"
//#include "../config.h"
#include "Arduino.h"


namespace EncoderTool
{
    constexpr unsigned N = 4;

    namespace // private
    {
        using relay_t = void (*)();

        callback_t callbacks[N]; // pointers to callback functions
        Encoder* states[N];      // state passed to callback functions
        relay_t relays[N];       // pointers to functions relaying calls from pin interrupts to the callback functions
        uint8_t pins[N];         // keep track which pin is attached to which callback

        // Functions to be attached to the pin interrupts. They relay the ISR to the actual callback
        template <unsigned nr>
        void relay()
        {
            callbacks[nr](states[nr]);
        }

        // intialize the array of relay functions recursively
        template <size_t N>  __attribute__((always_inline))
        inline void initRelays()
        {
            pins[N]   = UINT8_MAX;
            relays[N] = relay<N>;
            initRelays<N - 1>();
        }

        template <> __attribute__((always_inline))
        inline void initRelays<0>()
        {
            pins[0]   = UINT8_MAX;
            relays[0] = relay<0>;
        }

        bool initialized = false;
        void initialize()
        {
            Serial.println("initialize");
            initRelays<N - 1>();
            initialized = true;
        }

    } // private namespace

    bool attachInterruptEx(unsigned pin, callback_t callback, Encoder* state, int mode)
    {
        Serial.printf("attach %d\n", pin);
        if (!initialized) initialize();

        for (unsigned i = 0; i < N; i++)
        {
            if (pins[i] == UINT8_MAX || pins[i] == pin)
            {
                pins[i]      = pin;
                callbacks[i] = callback;
                states[i]    = state;
                attachInterrupt(pin, relays[i], mode);
                Serial.printf("ch: %d OK\n", i);
                return true;
            }
        }
        Serial.printf("ERR\n");
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