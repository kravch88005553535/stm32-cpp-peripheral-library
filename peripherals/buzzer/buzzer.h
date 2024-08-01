#ifndef __BUZZER_H__
#define __BUZZER_H__


#include "gpio_stm32f103.h"

class Buzzer
{
public:
  enum Channel
  {
    Channel_1, Channel_2,
    Channel_3, Channel_4
  };

  enum Frequency
  {
    Frequency_1000 = 1000,
    Frequency_1500 = 1500,
    Frequency_2000 = 2000,
  };
  
  enum Volume
  {
    Volume_0 = 0, Volume_1 = 1, Volume_2 = 2, Volume_3 = 3,
    Volume_4 = 10, Volume_5 = 15, Volume_6 = 20, Volume_7 = 25,
    Volume_8 = 30, Volume_9 = 35, Volume_10 = 100,
  };
  
  Buzzer(TIM_TypeDef& aref_timer, const Channel a_channel, const Frequency a_frequency,
       const Volume a_volume, const uint32_t a_timer_bus_clock);
  ~Buzzer();
  
  void   SetVolume(const Volume a_volume);
  Volume GetVolume() const;
  
  void       SetFrequency(const Frequency a_frequency);
  Frequency  GetFrequency() const;
  
  void MakeShortSound();
  void MakeLongSound();
  
  void Enable() const;
  void Disable() const;
  void Toggle() const;
  void Execute();
private:
  Buzzer() = delete;
  
  enum Status
  {
    Status_buzzer_disabled,
    Status_buzzer_is_making_short_sound,
    Status_buzzer_is_making_long_sound
  };
  
  TIM_TypeDef&    mref_timer;
  Channel         m_channel;
  Status          m_status;
  Frequency       m_frequency;
  Volume          m_volume;
  const uint32_t  m_timer_bus_clock;
};


#endif //__BUZZER_H__
