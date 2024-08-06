#include "i2c_stm32f103.h"

I2C::I2C(I2C_TypeDef* const ap_i2c, uint32_t a_i2c_clock, Speed a_speed, Address_format a_address_format)
  : mp_i2c{ap_i2c}
  , m_address_format{a_address_format}
{
  switch(reinterpret_cast<uint32_t>(mp_i2c))
  {
    case I2C1_BASE:
      RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
    break;
    
#if !defined (STM32F10X_LD) && !defined (STM32F10X_LD_VL)
    case I2C2_BASE:
      RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;
    break;
#endif //!defined (STM32F10X_LD) && !defined (STM32F10X_LD_VL)
  }

  mp_i2c->CR1 |= I2C_CR1_SWRST;
  mp_i2c->CR1 &= ~I2C_CR1_SWRST;
  
  mp_i2c->CR2 &= ~I2C_CR2_FREQ;
  mp_i2c->CR2 |= a_i2c_clock / 1'000'000;  
  SetSpeed(a_speed);
  mp_i2c->CR1 |= I2C_CR1_PE;
}

I2C::~I2C()
{
}

bool I2C::IsBusBusy()
{
  return mp_i2c->SR2 & I2C_SR2_BUSY;
}

void I2C::GenerateStartCondition()
{
  mp_i2c->CR1 |= I2C_CR1_START;
  while ((mp_i2c->SR1 & I2C_SR1_SB) != I2C_SR1_SB);
  mp_i2c->SR1;
}

void I2C::GenerateStopCondition()
{
  mp_i2c->CR1 |= I2C_CR1_STOP;
  while(IsBusBusy());
}

void I2C::TransmitDeviceAddress(const uint16_t a_address) //add r/2w bit & rewrite this function
{
  while(a_address > 0b1111111111)
  {/* ERROR! address is not in 10-bit range*/};
  
  switch (static_cast <uint32_t>(m_address_format))
  {
    case Address_format_7bit:
      mp_i2c->DR = (a_address & 0xFF);
      while (!(mp_i2c->SR1 & I2C_SR1_ADDR));
      mp_i2c->SR1;
      mp_i2c->SR2;
    break;
    
    case Address_format_10bit:
      constexpr uint8_t header_mask{0b11110000};
      const uint8_t header{static_cast<uint8_t>(((a_address & 0xFF00) >> 7) | header_mask)};
      mp_i2c->DR = header;
      mp_i2c->SR1;
      mp_i2c->DR = (a_address & 0xFF);
      while (!(mp_i2c->SR1 & I2C_SR1_ADD10));
      while (!(mp_i2c->SR1 & I2C_SR1_ADDR));
      mp_i2c->SR1;
      mp_i2c->SR2;
    break;
  }
}

void I2C::TransmitData(const uint8_t a_transmitdata)
{
  mp_i2c->DR = a_transmitdata;
  while (!(mp_i2c->SR1 & I2C_SR1_TXE));
}

void I2C::RecieveData(uint8_t* const ap_recievedata)
{
  mp_i2c->CR1 |= I2C_CR1_ACK;
  while (!(mp_i2c->SR1 & I2C_SR1_RXNE));
  *ap_recievedata = mp_i2c->DR;
}

void I2C::RecieveDataNoAck(uint8_t* const ap_recievedata)
{
  mp_i2c->CR1 &= ~I2C_CR1_ACK;
  while (!(mp_i2c->SR1 & I2C_SR1_RXNE));
  *ap_recievedata = mp_i2c->DR;
}

void I2C::CheckByteTransmissionFlag()
{
  while (!(mp_i2c->SR1 & I2C_SR1_BTF));
}

void I2C::SetSpeed(Speed a_speed)
{
  mp_i2c->CR1 &= ~I2C_CR1_PE;
  uint32_t apb_frequency = mp_i2c->CR2 & I2C_CR2_FREQ;
  const uint32_t Trise_plus_Tw_100kHz = 5000;

  switch (static_cast<uint32_t>(a_speed))
  {
    case Speed_100kHz:
      mp_i2c->CCR &= ~I2C_CCR_FS;
      mp_i2c->CCR &= ~I2C_CCR_CCR;
      mp_i2c->CCR = apb_frequency * Trise_plus_Tw_100kHz / 1000;
      mp_i2c->TRISE = apb_frequency + 1;
    break;
    
    case Speed_400kHz:
      mp_i2c->CCR |= I2C_CCR_FS;
      mp_i2c->CCR |= I2C_CCR_DUTY;
      mp_i2c->CCR &= ~I2C_CCR_CCR;
      mp_i2c->CCR |= apb_frequency * 100 /1000;
      mp_i2c->TRISE = (apb_frequency * 300 / 1000) + 1;
    break;
  }
  mp_i2c->CR1 |= I2C_CR1_PE;
}

I2C_TypeDef* const I2C::GetPtrI2C()
{
  return mp_i2c;
}