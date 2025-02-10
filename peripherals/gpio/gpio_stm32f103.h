#ifndef __GPIO_STM32F103__
#define __GPIO_STM32F103__

#include <stm32f10x.h>
#include <stdint.h>

class Pin
{
public:
  enum Mode: uint8_t
  {
    mode_in_floating = 0,
    mode_in_pullup,
    mode_in_pulldown,
    mode_out_pullup,
    mode_out_pulldown,
    mode_out_open_drain,
    mode_alternate_function_pushpull,
    mode_alternate_function_open_drain,
    mode_analog
  };
  
  enum Speed: uint8_t
  {
    Speed_2Mhz  = 0b10,
    Speed_10Mhz = 0b01,
    Speed_50Mhz = 0b11
  };
  
  explicit Pin(GPIO_TypeDef* const ap_port, const uint8_t a_pin_number, const Mode a_mode);
  ~Pin();
  void Set();
  void Reset();
  bool GetState() const;
  void Toggle();
  void SetMode(const Mode a_mode);
  Mode GetMode() const;
  void SetSpeed(const Speed a_speed);
  Speed GetSpeed() const;
  bool Lock();
  bool IsLocked();
private:
  Pin() = delete;
  GPIO_TypeDef* const mp_port;
  const uint8_t m_pin_number;
  Mode m_mode;
};

#endif //__GPIO_STM32F103__