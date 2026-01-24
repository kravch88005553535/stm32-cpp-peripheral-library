#ifndef __USART_H__
#define __USART_H__

#include <stm32f10x.h>
#include <cstdlib>

class Usart
{
  public:
  enum Interface: uint8_t
  {
    Interface_UART,
    Interface_USART,
    Interface_LIN,
    Interface_Smartcard
  };
  enum Baudrate
  {
    Baudrate_1200KBaud = 1200,
    Baudrate_2400KBaud = 2400,
    Baudrate_9600KBaud = 9600,
    Baudrate_10400KBaud = 10400,
    Baudrate_19200KBaud = 19200,
    Baudrate_38400KBaud = 38400,
    Baudrate_57600KBaud = 57600,
    Baudrate_115200KBaud = 115'200,
    Baudrate_230400KBaud = 230'400,
    Baudrate_460800KBaud = 460'800,
    Baudrate_921600KBaud = 921'600,
    Baudrate_2MBaud = 2'000'000,
    Baudrate_3MBaud = 3'000'000
  };
  enum WordLength: uint8_t
  {
    WordLength_8bits,
    WordLength_9bits
  };
  enum StopBits: uint8_t
  {
    StopBits_0_5,
    StopBits_1,
    StopBits_1_5,
    StopBits_2
  };
  enum Mode: uint8_t
  {
    Mode_TX,
    Mode_RX,
    Mode_Idle
  };
  enum ParityControl: uint8_t
  {
    ParityControl_disabled,
    ParityControl_enabled    
  };
  
  Usart(USART_TypeDef* ap_usart, Interface a_interface, WordLength a_wordlength, StopBits a_stopbits,
    ParityControl a_paritycontrol, Baudrate a_baudrate, uint32_t a_usart_clock);
  ~Usart();
  void SetInterface(Interface a_interface);
  void SetBaudrate(const Baudrate a_baudrate, const uint32_t a_usart_clock);
  void SetWordLength(Usart::WordLength a_wordlength);
  void SetStopBits(StopBits a_stopbits);
  void SetParityControl(ParityControl a_paritycontrol);
  uint32_t GetBaudrate();
  void EnableTransmitter();
  void DisableTransmitter();
  void EnableReciever();
  void DisableReciever();  
  void EnableDmaTransmitter();
  void DisableDmaTransmitter();
  void EnableDmaReciever();
  void DisableDmaReciever();
  
  bool IsTransferComplete();
  void ClearTransferCompleteFlag();
  bool IsIdleLineDetected();
  
  void Transmit(const char* ap_data);
  void Transmit(const uint32_t a_data);
  void Transmit(const uint8_t* ap_data, size_t a_size);
  void Recieve(uint8_t* ap_data);
  

  
  void ClearTerminal();
  uint32_t GetPeripheralAddress() const;
private:
  Usart() = delete;
  USART_TypeDef* mp_usart;
  Interface m_interface;
  Mode m_mode;
};

#endif //__USART_H__