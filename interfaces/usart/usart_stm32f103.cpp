#include <math.h>
#include "usart_stm32f103.h"


Usart::Usart(USART_TypeDef* ap_usart, Interface a_interface, WordLength a_wordlength, StopBits a_stopbits,
  ParityControl a_paritycontrol, Baudrate a_baudrate, uint32_t a_usart_clock)
  : mp_usart{ap_usart}
  , m_interface{a_interface}
{
  switch(reinterpret_cast<uint32_t>(ap_usart))
  {
    case USART1_BASE:
      RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
    break;
        
    case USART2_BASE:
      RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
     break;
   }
   
  mp_usart->CR1 |= USART_CR1_UE;
  SetWordLength(a_wordlength);
  SetStopBits(a_stopbits);
  SetParityControl(a_paritycontrol);
  
  SetBaudrate(a_baudrate, a_usart_clock);
  m_mode = Mode::Mode_Idle;
}

Usart::~Usart()
{
}

void Usart::SetInterface (Interface a_interface)
{
  m_interface = a_interface;
}


void Usart::SetWordLength(WordLength a_wordlength)
{
  switch(static_cast<uint32_t>(a_wordlength))
  {
    case WordLength_8bits:
      mp_usart->CR1 &= ~USART_CR1_M;
    break;
    
    case WordLength_9bits:
      mp_usart->CR1 |= USART_CR1_M;
    break;
  }
}

void Usart::SetStopBits(StopBits a_stopbits)
{
  if(m_interface==Interface_UART | m_interface==Interface_USART)
  {
    switch(static_cast<uint32_t>(a_stopbits))
    {
    case StopBits_1:
      mp_usart->CR2 &= ~USART_CR2_STOP;
    break;
    
    case StopBits_2:
      mp_usart->CR2 &= ~USART_CR2_STOP;
      mp_usart->CR2 |= USART_CR2_STOP_1;
    break;
    }
  }else
  {
    while(1);
  }
}

void Usart::SetBaudrate(const Baudrate a_baudrate, const uint32_t a_usart_clock)
{
  double baudrate = static_cast <double>(a_usart_clock) / 
    (static_cast<uint32_t>(a_baudrate) * 8 * (2 - static_cast<bool>(mp_usart->CR1 & USART_CR1_OVER8)));
  mp_usart->BRR = 0;

  mp_usart->BRR |=  static_cast<uint32_t>(baudrate) << 0x4U;
  uint8_t fraction =  static_cast<uint32_t>(round((baudrate - static_cast<uint32_t>(baudrate))*8)) & USART_BRR_DIV_Fraction;
  mp_usart->BRR |= fraction > 8 ? --fraction : fraction;
}

void Usart::SetParityControl(ParityControl a_paritycontrol)
{
  switch(static_cast<uint32_t>(a_paritycontrol))
  {
    case ParityControl_disabled:
      mp_usart->CR1 &= ~USART_CR1_PCE;
    break;
    
    case ParityControl_enabled:
      mp_usart->CR1 |= USART_CR1_PCE;
    break;
  }
}

uint32_t Usart::GetBaudrate()
{
//  rcc.GetPeripheralClock(mp_usart) / 8 / (2 - static_cast<bool>(mp_usart->CR1 & USART_CR1_OVER8)) / usartdiv
  return 0;
}

void Usart::EnableTransmitter()
{
  mp_usart->CR1 |= USART_CR1_TE;
}

void Usart::DisableTransmitter()
{
  mp_usart->CR1 &= ~USART_CR1_TE;
}

void Usart::EnableReciever()
{
  mp_usart->CR1 |= USART_CR1_RE;
}

void Usart::DisableReciever()
{
  mp_usart->CR1 &= ~USART_CR1_RE;
}

void Usart::EnableDmaTransmitter()
{
  mp_usart->CR3 |= USART_CR3_DMAT;
}
void Usart::DisableDmaTransmitter()
{
  mp_usart->CR3 &= ~USART_CR3_DMAT;
}
void Usart::EnableDmaReciever()
{
  mp_usart->CR3 |= USART_CR3_DMAR;
}

void Usart::DisableDmaReciever()
{
  mp_usart->CR3 &= ~USART_CR3_DMAR;
}

bool Usart::IsTransferComplete()
{
  return mp_usart->SR & USART_SR_TC;
}

void Usart::ClearTransferCompleteFlag()
{
  mp_usart->SR &= ~USART_SR_TC;
}

bool Usart::IsIdleLineDetected()
{
  return mp_usart->SR & USART_SR_IDLE;
}

void Usart::Transmit(const char* ap_data)
{
  m_mode = Mode::Mode_TX;
  EnableTransmitter();
  while(*ap_data)
  {
    while (!(mp_usart->SR & USART_SR_TXE));
    mp_usart->DR = *ap_data;
    ap_data++;
  }
  m_mode = Mode::Mode_Idle;
  DisableTransmitter();
}

void Usart::Transmit(const uint32_t a_data)
{
  char number_length{1};
  uint32_t temp_data = a_data;
  
  while(temp_data >= 10)
  {
    ++number_length;
    temp_data /= 10;
  }
  
  temp_data = a_data;
  EnableTransmitter();

  while(number_length--)
  {
    while (!(mp_usart->SR & USART_SR_TXE));
    uint16_t send_data = temp_data/pow(10,number_length) + 48;
    mp_usart->DR = send_data;
    temp_data = temp_data % uint32_t(pow(10, number_length));
  }
  
  DisableTransmitter();
}

void Usart::Transmit(const uint8_t* ap_data, size_t a_size)
{
  EnableTransmitter();
  while(a_size--)
  {
    while (!(mp_usart->SR & USART_SR_TXE));
    mp_usart->DR = *ap_data;
    ap_data++;
  }
  DisableTransmitter();
}

void Usart::ClearTerminal()
{
  constexpr char array[] = {0x1B, 0x5B, 0x32, 0x4A, 0};
  Transmit(array);
}

uint32_t Usart::GetPeripheralAddress() const 
{
  return reinterpret_cast<uint32_t>(mp_usart);
}