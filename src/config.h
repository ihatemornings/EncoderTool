#pragma once

// un-comment the following line if you prefer plain function pointers for callbacks

//================================================================================================================




//#include "Arduino.h"


extern void d_write(uint32_t pin, uint32_t val);
extern int32_t d_read(uint32_t pin, uint32_t val);

#if defined(TEENSYDUINO)
#include "platforms/teensyduino.h"

#elif defined(ARDUINO_ARCH_STM32)
#include "platforms/stm32.h"

#elif defined(ARDUINO_ARCH_SAMD)
#include "platforms/samd.h"

constexpr unsigned nrOfInterruptPins = EXTERNAL_NUM_INTERRUPTS;
//constexpr unsigned nrOfInterruptPins = NUM_DIGITAL_PINS;

//#include "platforms/pin.h"
namespace EncoderTool
{
  using Pin = pin_t<uint32_t>;
}

#define PLAIN_ENC_CALLBACK

#endif

#if not defined(PLAIN_ENC_CALLBACK)
#include <functional>
#endif


namespace EncoderTool
{
#if defined(PLAIN_ENC_CALLBACK)
  using encCallback_t = void (*)(int32_t value, int32_t delta);
  using encBtnCallback_t = void (*)(int32_t state);

  using allCallback_t = void (*)(uint32_t channel, int32_t value, int32_t delta);
  using allBtnCallback_t = void (*)(int32_t state);
#else
  using encCallback_t = std::function<void(int32_t value, int32_t delta)>; // encoder value
  using encBtnCallback_t = std::function<void(int32_t state)>;             // encoder button

  using allCallback_t = std::function<void(uint32_t channel, int32_t value, int32_t delta)>; // all encoder values
  using allBtnCallback_t = std::function<void(uint32_t channel, int32_t state)>;             // all encoder buttons
#endif
}

