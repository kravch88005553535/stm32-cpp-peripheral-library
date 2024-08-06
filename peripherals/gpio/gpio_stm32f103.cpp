#include "gpio_stm32f103.h"

Pin::Pin(GPIO_TypeDef* const ap_port, const uint8_t a_pin_number, const Mode a_mode)
  : mp_port{ap_port}
  , m_pin_number{a_pin_number}
  , m_mode{a_mode}
{
  switch(reinterpret_cast<uint32_t>(mp_port))
  {
    case GPIOA_BASE:
      RCC->APB2ENR |=  RCC_APB2ENR_IOPAEN;
    break;

    case GPIOB_BASE:
      RCC->APB2ENR |=  RCC_APB2ENR_IOPBEN;
    break;
    
    case GPIOC_BASE:
      RCC->APB2ENR |=  RCC_APB2ENR_IOPCEN;
    break;
    
    case GPIOD_BASE:
      RCC->APB2ENR |=  RCC_APB2ENR_IOPDEN;
    break;
    
#if !defined (STM32F10X_LD) && !defined (STM32F10X_LD_VL)
    case GPIOE_BASE:
      RCC->APB2ENR |= RCC_APB2ENR_IOPEEN;
    break;
#endif /* STM32F10X_LD && STM32F10X_LD_VL */
    
#if defined (STM32F10X_HD) || defined (STM32F10X_XL)
    case GPIOF_BASE:
      RCC->APB2ENR |= RCC_APB2ENR_IOPFEN;
    break;
    
    case GPIOG_BASE:
      RCC->APB2ENR |= RCC_APB2ENR_IOPGEN;
    break;
#endif //defined (STM32F10X_HD) || defined (STM32F10X_XL)
  }
  SetMode(m_mode);
}

Pin::~Pin()
{
}
void Pin::Set()
{
  mp_port->BSRR = GPIO_BSRR_BS0 << m_pin_number;
}
void Pin::Reset()
{
  mp_port->BRR = GPIO_BRR_BR0 << m_pin_number;
}

bool Pin::GetState() const
{
  return mp_port->IDR & GPIO_IDR_IDR0 << m_pin_number;
}
void Pin::Toggle()
{
  mp_port->ODR ^= GPIO_ODR_ODR0 << m_pin_number;
}

void Pin::SetMode(const Mode a_mode)
{
//dont check equation of a_mode == m_mode, because this function calls from the constructor!
  if(IsLocked())
    return;

  switch(a_mode)
  {
    case Mode::mode_in_floating:  
    {
      if(m_pin_number < 8)
      {
        mp_port->CRL &= ~(GPIO_CRL_MODE0 << (m_pin_number*4));          //MODE_INPUT
        mp_port->CRL &= ~(GPIO_CRL_CNF0  << (m_pin_number*4));          //ANALOG_MODE //clear
        mp_port->CRL |= GPIO_CRL_CNF0_0  << (m_pin_number*4);           //FLOATING_INPUT
      }
      else
      {
        mp_port->CRH &= ~(GPIO_CRH_MODE8 << ((m_pin_number-8)*4));      //MODE_INPUT  
        mp_port->CRH &= ~(GPIO_CRH_CNF8 << ((m_pin_number-8)*4));       //ANALOG_MODE //clear
        mp_port->CRH |= GPIO_CRH_CNF8_0 << ((m_pin_number-8)*4);        //FLOATING_INPUT
      }
    }
    break;
    
    case Mode::mode_in_pullup:
    {
      if(m_pin_number < 8)
      {
        mp_port->CRL &= ~(GPIO_CRL_MODE0 << (m_pin_number*4));          //MODE_INPUT  
        mp_port->CRL &= ~(GPIO_CRL_CNF0  << (m_pin_number*4));          //ANALOG_MODE //clear
        mp_port->CRL |= GPIO_CRL_CNF0_1  << (m_pin_number*4);           //INPUT WITN PU_PD
      }
      else
      {
        mp_port->CRH &= ~(GPIO_CRH_MODE8 << ((m_pin_number-8)*4));      //MODE_INPUT  
        mp_port->CRH &= ~(GPIO_CRH_CNF8 << ((m_pin_number-8)*4));       //ANALOG_MODE //clear
        mp_port->CRH |= GPIO_CRH_CNF8_1 << ((m_pin_number-8)*4);        //INPUT WITN PU_PD
      }
      mp_port->ODR |= GPIO_ODR_ODR0 << m_pin_number;
    }
    break;
    
    case Mode::mode_in_pulldown:
    {
      if(m_pin_number < 8)
      {
        mp_port->CRL &= ~(GPIO_CRL_MODE0 << (m_pin_number*4));          //MODE_INPUT  
        mp_port->CRL &= ~(GPIO_CRL_CNF0  << (m_pin_number*4));          //ANALOG_MODE //clear
        mp_port->CRL |= GPIO_CRL_CNF0_1  << (m_pin_number*4);           //INPUT WITN PU_PD
      }
      else
      {
        mp_port->CRH &= ~(GPIO_CRH_MODE8 << ((m_pin_number-8)*4));      //MODE_INPUT  
        mp_port->CRH &= ~(GPIO_CRH_CNF8 << ((m_pin_number-8)*4));       //ANALOG_MODE //clear
        mp_port->CRH |= GPIO_CRH_CNF8_1 << ((m_pin_number-8)*4);        //INPUT WITN PU_PD
      }
      mp_port->ODR &= ~(GPIO_ODR_ODR0 << m_pin_number);
    }  
    break;
    
    case Mode::mode_out_pullup:
    {
      if(m_pin_number < 8)
      {
        mp_port->CRL |= GPIO_CRL_MODE0 << (m_pin_number*4);               //MODE_OUTPUT, max_speed
        mp_port->CRL &= ~(GPIO_CRL_CNF0  << (m_pin_number*4));            //OUT_PP
      }
      else
      {
        mp_port->CRH |= GPIO_CRH_MODE8 << ((m_pin_number-8)*4);            //MODE_OUTPUT, max_speed
        mp_port->CRH &= ~(GPIO_CRH_CNF8 << ((m_pin_number-8)*4));          //OUT_PP
      }      
      mp_port->ODR |= GPIO_ODR_ODR0 << m_pin_number;                       //OUT_PU
    }
    break;
    
    case Mode::mode_out_pulldown:
    {
      if(m_pin_number < 8)
      {
        mp_port->CRL |= GPIO_CRL_MODE0 << (m_pin_number*4);                //MODE_OUTPUT, max_speed
        mp_port->CRL &= ~(GPIO_CRL_CNF0  << (m_pin_number*4));             //OUT_PP
      }
      else
      {
        mp_port->CRH |= GPIO_CRH_MODE8 << ((m_pin_number-8)*4);            //MODE_OUTPUT, max_speed
        mp_port->CRH &= ~(GPIO_CRH_CNF8 << ((m_pin_number-8)*4));          //OUT_PP
      }      
      mp_port->ODR &= ~(GPIO_ODR_ODR0 << m_pin_number);                    //OUT_PD
    }  
    break;
        
    case Mode::mode_out_open_drain:
    {
      if(m_pin_number < 8)
      {
        mp_port->CRL |= GPIO_CRL_MODE0 << (m_pin_number*4);               //MODE_OUTPUT, max_speed
        mp_port->CRL &= ~(GPIO_CRL_CNF0  << (m_pin_number*4));            //CLEAR_CNF
        mp_port->CRL |= GPIO_CRL_CNF0_0  << (m_pin_number*4);             //OUTPUT_OD
      }
      else
      {
        mp_port->CRH |= GPIO_CRH_MODE8 << ((m_pin_number-8)*4);            //MODE_OUTPUT, max_speed
        mp_port->CRH &= ~(GPIO_CRH_CNF8 << ((m_pin_number-8)*4));          //CLEAR_CNF
        mp_port->CRH |= GPIO_CRH_CNF8_0 << ((m_pin_number-8)*4);           //OUTPUT_OD
      }
    }
    break;
    
    case Mode::mode_alternate_function_pushpull:
    {
      if(m_pin_number < 8)
      {
        mp_port->CRL |= GPIO_CRL_MODE0 << (m_pin_number*4);                //MODE_OUTPUT, max_speed
        mp_port->CRL &= ~(GPIO_CRL_CNF0  << (m_pin_number*4));             //CLEAR_CNF
        mp_port->CRL |= GPIO_CRL_CNF0_1  << (m_pin_number*4);              //OUTPUT_AF_PP
      }
      else
      {
        mp_port->CRH |= GPIO_CRH_MODE8 << ((m_pin_number-8)*4);            //MODE_OUTPUT, max_speed
        mp_port->CRH &= ~(GPIO_CRH_CNF8  << (m_pin_number-8)*4);           //CLEAR_CNF
        mp_port->CRH |= GPIO_CRH_CNF8_1 << ((m_pin_number-8)*4);           //OUTPUT_AF_PP
      }
    }
    break;
    
    case Mode::mode_alternate_function_open_drain:
    {
      if(m_pin_number < 8)
      {
        mp_port->CRL |= GPIO_CRL_MODE0 << (m_pin_number*4);                //MODE_OUTPUT, max_speed
        mp_port->CRL |= GPIO_CRL_CNF0  << (m_pin_number*4);                //OUTPUT_AF_OD
      }
      else
      {
        mp_port->CRH |= GPIO_CRH_MODE8 << ((m_pin_number-8)*4);            //MODE_OUTPUT, max_speed
        mp_port->CRH |= GPIO_CRH_CNF8 << ((m_pin_number-8)*4);             //OUTPUT_AF_OD
      }
    }
    break;
    
    case Mode::mode_analog:
    {
      if(m_pin_number < 8)
      {
        mp_port->CRL &= ~(GPIO_CRL_MODE0 << (m_pin_number*4));          //MODE_INPUT
        mp_port->CRL &= ~(GPIO_CRL_CNF0  << (m_pin_number*4));          //ANALOG_MODE 
      }
      else
      {
        mp_port->CRH &= ~(GPIO_CRH_MODE8 << ((m_pin_number-8)*4));      //MODE_INPUT  
        mp_port->CRH &= ~(GPIO_CRH_CNF8 << ((m_pin_number-8)*4));       //ANALOG_MODE 
      }
    }
    break;
  }
  m_mode = a_mode;
}

Pin::Mode Pin::GetMode() const
{
  return m_mode;
}

bool Pin::Lock()
{
  volatile uint32_t lock_register{mp_port->LCKR | GPIO_LCKR_LCKK | GPIO_LCKR_LCK0 << m_pin_number};
  
  mp_port->LCKR = lock_register; //LCKK is 1
  lock_register &= ~GPIO_LCKR_LCKK;
  mp_port->LCKR = lock_register; //LCKK is 0
  lock_register |= GPIO_LCKR_LCKK;
  mp_port->LCKR = lock_register; //LCKK is 1
  
  lock_register = mp_port->LCKR; //read LCKR
  
  return mp_port->LCKR & GPIO_LCKR_LCKK;
}

bool Pin::IsLocked()
{
  return mp_port->LCKR & GPIO_LCKR_LCK0 << m_pin_number;
}