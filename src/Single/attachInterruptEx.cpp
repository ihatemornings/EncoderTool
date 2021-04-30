#include <array>
#include <utility>
#include "attachInterruptEx.h"
#include "Arduino.h"

using namespace std;

namespace EncoderTool
{
    // We need to attach a memberfunction of the encoder class to a pin interrupt.
    // Since attachInterrupt only accepts state less void(*)(void) callbacks this is not possible directly.
    // Instead,we generate an array of void(*)() relay functions to attach to the pin interrupts. Those relays will invoke the actual callbacks.
    // see https://stackoverflow.com/q/63730007/1842762

    // ! We don't use <std::function> here (EncoderTool) to allow usage from smaller MCUs like TeensyLC !

    cb_t callbacks[CORE_NUM_INTERRUPT]; // storage for the callback functions
    state_t states[CORE_NUM_INTERRUPT]; // storage for the corresponding state variables

    template <unsigned nr>
    constexpr void relay()
    {
        callbacks[nr](states[nr]);
    }

    template <unsigned... nr>
    constexpr array<void (*)(), CORE_NUM_INTERRUPT> MakeRelays(index_sequence<nr...>)
    {
        return array<void (*)(), CORE_NUM_INTERRUPT>{relay<nr>...};
    }

    constexpr auto relays = MakeRelays(make_index_sequence<CORE_NUM_INTERRUPT>{});  // generates the relay function array at compile time

    void attachInterruptEx(unsigned pin, cb_t callback, state_t state, int mode)
    {
        states[pin] = state;
        callbacks[pin] = callback;
        attachInterrupt(pin, relays[pin], mode);
    }
}