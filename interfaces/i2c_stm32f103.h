#ifndef __I2C_H__
#define __I2C_H__

#include <stm32f10x.h>

class I2C_Interface
{
public:
virtual void Lock() = 0;
virtual void Unlock() = 0;
virtual bool IsLocked() = 0;
};

class I2C: public I2C_Interface 
{
public:
  enum Interface: uint8_t
  {
    Interface_I2C,
    Interface_SMBUS
  };
  enum Status: uint8_t
  {  
    Status_Master_Transmitter,
    Status_Master_Reciever,
    Status_Slave_Transmitter,
    Status_Slave_Reciever
  };
  enum Mode: uint8_t
  {
    Mode_Sm,
    Mode_Fm
  };
  enum Address_format: uint8_t
  {
    Address_format_7bit,
    Address_format_10bit
  };
  enum Speed: uint8_t
  {
    Speed_100kHz,
    Speed_400kHz
  };
  
  I2C(I2C_TypeDef* const ap_i2c, uint32_t a_i2c_clock, Speed a_speed, Address_format a_address_format);
  ~I2C();

  bool IsBusBusy();
  void GenerateStartCondition();
  void GenerateStopCondition();
  void TransmitDeviceAddress(const uint16_t a_address);
  void TransmitData(const uint8_t a_transmitdata);
  void RecieveData(uint8_t* const ap_recievedata);
  void RecieveDataNoAck(uint8_t* const ap_recievedata);
  void CheckByteTransmissionFlag();
  void SetSpeed(Speed a_speed);
  I2C_TypeDef* const GetPtrI2C();
private:  
  Address_format m_address_format;
  I2C_TypeDef* const mp_i2c;
};

#endif //__I2C_H__