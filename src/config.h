#pragma once

#include <cstdint>


extern void d_write(uint32_t pin, uint32_t val);
extern int32_t d_read(uint32_t pin, uint32_t val);

#if defined(TEENSYDUINO)
#include "platforms/teensyduino.h"

#elif defined(ARDUINO_ARCH_STM32)
#include "platforms/stm32.h"

#elif defined(ARDUINO_ARCH_SAMD)
#include "platforms/samd.h"

#elif defined(ARDUINO_ARCH_ESP32)
#include "platforms/esp32.h"

#endif


#if !defined(PLAIN_ENC_CALLBACK)
#    include <functional>
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

