#pragma once

#include "Arduino.h"

inline void d_write(uint32_t pin, uint32_t val)
{
  digitalWriteFast(pin, val);
}

inline int d_read(uint32_t pin)
{
  return digitalReadFast(pin);
}