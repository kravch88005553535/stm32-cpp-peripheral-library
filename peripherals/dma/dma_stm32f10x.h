#ifndef __DMA_H__
#define __DMA_H__

#include <stm32f10x.h>

class STM32_DMA
{

public:
//  enum DMA_Channel : uint8_t
//  {
//    DMA_Channel_1_1 = 0,
//    DMA_Channel_1_2 = 1,
//    DMA_Channel_1_3 = 2,
//    DMA_Channel_1_4 = 3,
//    DMA_Channel_1_5 = 4,
//    DMA_Channel_1_6 = 5,
//    DMA_Channel_1_7 = 6,
//    
//    DMA_Channel_2_1 = 0,
//    DMA_Channel_2_2 = 1,
//    DMA_Channel_2_3 = 2,
//    DMA_Channel_2_4 = 3,
//    DMA_Channel_2_5 = 4,
//  };
  
  enum Mode : uint8_t
  {
    Mode_Memory2Memory,
    Mode_Memory2Peripheral,
    Mode_Peripheral2Memory,
  };
  
  enum Interrupt : uint8_t
  {
    Interrupt_TransferError,
    Interrupt_HalfTransfer,
    Interrupt_TransferComplete,
    Interrupt_GlobalInterrupt
  };
  
  enum ChannelPriority : uint8_t
  {
    ChannelPriority_low,
    ChannelPriority_medium,
    ChannelPriority_high,
    ChannelPriority_veryhigh
  };
  
  enum MemorySize : uint8_t
  {
    MemorySize_8bit,
    MemorySize_16bit,
    MemorySize_32bit
  };
  
  enum PeripheralSize : uint8_t
  {
    PeripheralSize_8bit,
    PeripheralSize_16bit,
    PeripheralSize_32bit
  };
  
private:

public:
  STM32_DMA(DMA_Channel_TypeDef* ap_dma_channel = nullptr);
  ~STM32_DMA();
  
  void SetChannel(DMA_Channel_TypeDef* ap_dma_channel);
  void EnableChannel();
  void EnableChannel(DMA_Channel_TypeDef* const  ap_dma_channel);
  void DisableChannel();
  void EnableInterrupt(const Interrupt a_interrupt);
  void DisableInterrupt(const Interrupt a_interrupt);
  void SetMode(const Mode a_mode);
  void SetChannelPriority(const ChannelPriority a_priority);
  
  void SetTransferSize(const uint16_t a_size);
  void EnableCircularMode();
  void DisableCircularMode();

  void SetPeripheralAddress(const uint32_t a_address);
  void SetMemoryAddress (const uint32_t a_address);
  void SetPeripheralSize(const PeripheralSize a_size);
  void SetMemorySize(const MemorySize a_size);
  void EnablePeripheralIncrement();
  void DisablePeripheralIncrement();
  void EnableMemoryIncrement();
  void DisableMemoryIncrement();
  
  bool IsBusy();
  void ClearFlag(); // TC, TE, HTC
  bool IsTransferComplete();
  bool IsHalfTransferComplete();
private:
  DMA_Channel_TypeDef* mp_dma_channel;
  
};

#endif  //__DMA_H__