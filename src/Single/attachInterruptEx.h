#pragma once

namespace EncoderTool
{
    class Encoder;
    using callback_t = void (*)(Encoder* state); // callback type

    bool attachInterruptEx(unsigned pin, callback_t callback, Encoder* state, int mode);
    bool detachInterruptEx(unsigned pin);
}