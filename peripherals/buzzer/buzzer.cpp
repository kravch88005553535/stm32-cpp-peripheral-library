#include "buzzer.h"

Buzzer::Buzzer(TIM_TypeDef& aref_timer, const Channel a_channel, const Frequency a_frequency,
               const Volume a_volume, const uint32_t a_timer_bus_clock)
  : mref_timer{aref_timer}
  , m_channel{a_channel}
  , m_frequency{a_frequency}
  , m_volume{a_volume}
  , m_timer_bus_clock{a_timer_bus_clock}
{
  switch(reinterpret_cast<uint32_t>(&mref_timer))
  {
    case TIM1_BASE:
      RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
    break;
  
#if defined (STM32F10X_HD) || defined (STM32F10X_XL)
    case TIM8_BASE:
      RCC->APB2ENR |= RCC_APB2ENR_TIM8EN;
    break;
#endif
    
    case TIM2_BASE:
      RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    break;
    
    case TIM3_BASE:
      RCC->APB1ENR |=RCC_APB1ENR_TIM3EN;
    break;
  
#if !defined (STM32F10X_LD) && !defined (STM32F10X_LD_VL)
    case TIM4_BASE:
      RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
    break;
#endif

#if defined (STM32F10X_HD) || defined  (STM32F10X_CL)
    case TIM5_BASE:
      RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;
    break;
#endif

#ifdef STM32F10X_XL
    case TIM9_BASE:
      RCC->APB2ENR |= RCC_APB2ENR_TIM9EN;
    break;
    
    case TIM10_BASE:
      RCC->APB2ENR |= RCC_APB2ENR_TIM10EN;
    break;
    
    case TIM11_BASE:
      RCC->APB2ENR |= RCC_APB2ENR_TIM11EN;
    break;
#endif
  
#ifdef STM32F10X_HD_VL
    case TIM12_BASE:
      RCC->APB1ENR |= RCC_APB1ENR_TIM12EN;
    break;
    
    case TIM13_BASE:
      RCC->APB1ENR |= RCC_APB1ENR_TIM13EN;
    break;
    
    case TIM14_BASE:
      RCC->APB1ENR |= RCC_APB1ENR_TIM14EN;
    break;
#endif
  
    default:
      while(1);
    break;
  }
  
//  load parameters from eeprom
  
  mref_timer.PSC = 10-1;
  
  SetFrequency(m_frequency);
  
  mref_timer.ARR = 14400-1;
  mref_timer.CCR1 = (mref_timer.ARR + 1) / 2;
  
  mref_timer.CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2;
  mref_timer.CCMR1 |= TIM_CCMR1_OC1PE;
  mref_timer.CCER  |= TIM_CCER_CC1E;
  //enable output
  switch(m_channel)
  {
    case Channel_1:
    break;
    case Channel_2:
    break;
    case Channel_3:
    break;
    case Channel_4:
    break;
  }
  
  mref_timer.CR1 |= TIM_CR1_ARPE;
  mref_timer.EGR |= TIM_EGR_UG;
  mref_timer.CR1 |= TIM_CR1_CEN;
}

void Buzzer::SetVolume(const Volume a_volume)
{
  m_volume = a_volume;
  
  uint16_t ccr_value = (mref_timer.ARR + 1) / 2 * m_volume / 100;
  switch(m_channel)
  {
    case Channel_1:
      mref_timer.CCR1 = ccr_value;
    break;
    case Channel_2:
      mref_timer.CCR2 = ccr_value;
    break;
    case Channel_3:
      mref_timer.CCR3 = ccr_value;
    break;
    case Channel_4:
      mref_timer.CCR4 = ccr_value;
    break;
  }
}

Buzzer::Volume Buzzer::GetVolume() const
{
  return m_volume;
}
void Buzzer::SetFrequency(const Frequency a_frequency)
{
  m_frequency = a_frequency;
  mref_timer.ARR = m_timer_bus_clock / a_frequency / (mref_timer.PSC + 1);
}

Buzzer::Frequency Buzzer::GetFrequency() const
{
  return m_frequency;
}
  
void Buzzer::MakeShortSound()
{
  if(m_status != Status_buzzer_is_making_long_sound)
  {
    m_status = Status_buzzer_is_making_short_sound;
    //start program_timer
  }
}
void Buzzer::MakeLongSound()
{
  m_status = Status_buzzer_is_making_long_sound;
  //start program_timer
}

void Buzzer::Enable() const
{
  mref_timer.BDTR |= TIM_BDTR_MOE;
}
void Buzzer::Disable() const
{
  mref_timer.BDTR &= ~TIM_BDTR_MOE;
}

void Buzzer::Toggle() const
{
  if(mref_timer.BDTR & TIM_BDTR_MOE)
    mref_timer.BDTR &= ~TIM_BDTR_MOE;
  else 
    mref_timer.BDTR |= TIM_BDTR_MOE;
}

void Buzzer::Execute()
{
}