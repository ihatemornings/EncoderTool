#include "attachInterruptEx.h"
#include "../config.h"
#include "Arduino.h"
#include <array>
#include <utility>

using namespace std;

namespace EncoderTool
{
    // We need to attach a memberfunction of the encoder class to a pin interrupt.
    // Since attachInterrupt only accepts state less void(*)(void) callbacks this is not possible directly.
    // Instead,we generate an array of void(*)() relay functions to attach to the pin interrupts. Those relays will invoke the actual callbacks.
    // see https://stackoverflow.com/q/63730007/1842762

    // ! We don't use <std::function> here (EncoderTool) to allow usage from smaller MCUs like TeensyLC !

    cb_t callbacks[nrOfInterruptPins]; // storage for the callback functions
    state_t states[nrOfInterruptPins]; // storage for the corresponding state variables

    template <unsigned nr>
    constexpr void relay()
    {
        callbacks[nr](states[nr]);
    }

    template <unsigned... nr>
    constexpr array<void (*)(), nrOfInterruptPins> MakeRelays(index_sequence<nr...>)
    {
        return array<void (*)(), nrOfInterruptPins>{relay<nr>...};
    }

    constexpr auto relays = MakeRelays(make_index_sequence<nrOfInterruptPins>{}); // generates the relay function array at compile time

    void attachInterruptEx(unsigned pin, cb_t callback, state_t state, int mode)
    {
        states[pin]    = state;
        callbacks[pin] = callback;
        attachInterrupt(pin, relays[pin], mode);
    }
}