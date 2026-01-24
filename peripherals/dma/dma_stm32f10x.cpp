#include <stdio.h>
#include "/peripherals/dma/dma_stm32f10x.h"

STM32_DMA::STM32_DMA(DMA_Channel_TypeDef* ap_dma_channel)
  : mp_dma_channel{ap_dma_channel}
{
  RCC->AHBENR |= RCC_AHBENR_DMA1EN;
  
#if defined (STM32F10X_HD) || defined (STM32F10X_XL) || defined  (STM32F10X_CL) || defined  (STM32F10X_HD_VL)
  RCC->AHBENR |= RCC_AHBENR_DMA2EN;
#endif // defined (STM32F10X_HD) || defined (STM32F10X_XL) || defined  (STM32F10X_CL) || defined  (STM32F10X_HD_VL)
  
}

void STM32_DMA::SetChannel(DMA_Channel_TypeDef* ap_dma_channel)
{
  mp_dma_channel = ap_dma_channel;
}

void STM32_DMA::EnableChannel()
{
  if(mp_dma_channel == nullptr)
  {
    printf("DMA channel is not assigned");
    return;
  }
  constexpr auto DMA_CCRx_EN{DMA_CCR1_EN};
  mp_dma_channel->CCR |= DMA_CCRx_EN;
}

void STM32_DMA::EnableChannel(DMA_Channel_TypeDef* const ap_dma_channel)
{
  if(mp_dma_channel == nullptr)
  {
    printf("DMA channel is nullptr");
    return;
  }
  
  constexpr auto DMA_CCRx_EN{DMA_CCR1_EN};
  ap_dma_channel->CCR |= DMA_CCRx_EN;
}

void STM32_DMA::DisableChannel()
{
  if(mp_dma_channel == nullptr)
  {
    printf("DMA channel is not assigned");
    return;
  }
  constexpr auto DMA_CCRx_EN{DMA_CCR1_EN};
  mp_dma_channel->CCR &= ~DMA_CCRx_EN;
}

void STM32_DMA::EnableInterrupt(const Interrupt a_interrupt)
{
  constexpr auto DMA_CCRx_TEIE{DMA_CCR1_TEIE};
  constexpr auto DMA_CCRx_HTIE{DMA_CCR1_HTIE};
  constexpr auto DMA_CCRx_TCIE{DMA_CCR1_TCIE};
  
  switch (a_interrupt)
  {
    case Interrupt_TransferError:
      mp_dma_channel->CCR |= DMA_CCRx_TEIE;
    break;
    
    case Interrupt_HalfTransfer:
      mp_dma_channel->CCR |= DMA_CCRx_HTIE;
    break;
    
    case Interrupt_TransferComplete:
      mp_dma_channel->CCR |= DMA_CCRx_TCIE;
    break;
    
    case Interrupt_GlobalInterrupt:  
//      NVIC_EnableIRQ(DMA1_Channel1_IRQn);
    break;
  }
}

void STM32_DMA::DisableInterrupt(const Interrupt a_interrupt)
{
  constexpr auto DMA_CCRx_TEIE{DMA_CCR1_TEIE};
  constexpr auto DMA_CCRx_HTIE{DMA_CCR1_HTIE};
  constexpr auto DMA_CCRx_TCIE{DMA_CCR1_TCIE};
  
  switch (a_interrupt)
  {
    case Interrupt_TransferError:
      mp_dma_channel->CCR &= ~DMA_CCRx_TEIE;
    break;
    
    case Interrupt_HalfTransfer:
      mp_dma_channel->CCR &= ~DMA_CCRx_HTIE;
    break;
    
    case Interrupt_TransferComplete:
      mp_dma_channel->CCR &= ~DMA_CCRx_TCIE;
    break;
    
    case Interrupt_GlobalInterrupt:  
//      NVIC_EnableIRQ(DMA1_Channel1_IRQn);
    break;
  }
}

void STM32_DMA::SetMode(const Mode a_mode)
{
  constexpr auto DMA_CCRx_MEM2MEM{DMA_CCR1_MEM2MEM};
  constexpr auto DMA_CCRx_DIR{DMA_CCR1_DIR};
  
  switch(a_mode)
  {
    case Mode_Memory2Memory:
      mp_dma_channel->CCR |= DMA_CCRx_MEM2MEM;
      mp_dma_channel->CCR |= DMA_CCRx_DIR;
    break;
    
    case Mode_Memory2Peripheral:
      mp_dma_channel->CCR &= ~DMA_CCRx_MEM2MEM;
      mp_dma_channel->CCR |= DMA_CCRx_DIR;
    break;
    
    case Mode_Peripheral2Memory:
      mp_dma_channel->CCR &= ~DMA_CCRx_MEM2MEM;
      mp_dma_channel->CCR &= ~DMA_CCRx_DIR;
    break;
  }
}

void STM32_DMA::SetChannelPriority(const ChannelPriority a_priority)
{
  constexpr uint16_t DMA_CCRx_PL{DMA_CCR1_PL};
  constexpr uint16_t DMA_CCRx_PL_0{DMA_CCR1_PL_0};
  constexpr uint16_t DMA_CCRx_PL_1{DMA_CCR1_PL_1};
  
  switch(a_priority)
  {
    case ChannelPriority_low:
      mp_dma_channel->CCR &= ~DMA_CCRx_PL;
    break;
    
    case ChannelPriority_medium:
      mp_dma_channel->CCR &= ~DMA_CCRx_PL;
      mp_dma_channel->CCR |= DMA_CCR1_PL_0;
    break;
    
    case ChannelPriority_high:
      mp_dma_channel->CCR &= ~DMA_CCRx_PL;
      mp_dma_channel->CCR |= DMA_CCR1_PL_1;
    break;
    
    case ChannelPriority_veryhigh:
      mp_dma_channel->CCR |= DMA_CCR1_PL;
    break;  
  }
}

void STM32_DMA::SetTransferSize(const uint16_t a_size)
{
  constexpr auto DMA_CCRx_EN {DMA_CCR1_EN};
  
  mp_dma_channel->CCR &= ~DMA_CCRx_EN;
  mp_dma_channel->CNDTR = a_size;
}

void STM32_DMA::SetPeripheralAddress(const uint32_t a_address)
{
  mp_dma_channel->CPAR = a_address;
}

void STM32_DMA::SetMemoryAddress (const uint32_t a_address)
{
  mp_dma_channel->CMAR = a_address;
}

void STM32_DMA::SetPeripheralSize(const PeripheralSize a_size)
{
  constexpr uint16_t DMA_CCRx_PSIZE{DMA_CCR1_PSIZE};
  constexpr auto DMA_CCRx_PSIZE_Pos = 8;
  switch (a_size)
  {
    case PeripheralSize_8bit:
      mp_dma_channel->CCR &= ~DMA_CCRx_PSIZE;
    break;
    
    case PeripheralSize_16bit:
    case PeripheralSize_32bit:
      mp_dma_channel->CCR &= ~DMA_CCRx_PSIZE;
      mp_dma_channel->CCR |= a_size << DMA_CCRx_PSIZE_Pos;
    break;
  }
}

void STM32_DMA::SetMemorySize(const MemorySize a_size)
{
  ////////////////////////////////////////////////////////////////////////////////EDIT
  constexpr uint16_t DMA_CCRx_MSIZE{DMA_CCR1_MSIZE};
  constexpr auto DMA_CCRx_MSIZE_Pos = 8;
  switch (a_size)
  {
    case MemorySize_8bit:
    break;
    
    case MemorySize_16bit:
    break;
    
    case MemorySize_32bit:
    break;
  }
}

void STM32_DMA::EnablePeripheralIncrement()
{
  constexpr uint16_t DMA_CCRx_PINC{DMA_CCR1_PINC};
  mp_dma_channel->CCR |= DMA_CCRx_PINC;
}

void STM32_DMA::DisablePeripheralIncrement()
{
  constexpr uint16_t DMA_CCRx_PINC{DMA_CCR1_PINC};
  mp_dma_channel->CCR &= ~DMA_CCRx_PINC;
}

void STM32_DMA::EnableMemoryIncrement()
{
  constexpr uint16_t DMA_CCRx_MINC{DMA_CCR1_MINC};
  mp_dma_channel->CCR |= DMA_CCRx_MINC;
}

void STM32_DMA::DisableMemoryIncrement()
{
  constexpr uint16_t DMA_CCRx_MINC{DMA_CCR1_MINC};
  mp_dma_channel->CCR &= ~DMA_CCRx_MINC;
}

void STM32_DMA::EnableCircularMode()
{
  constexpr uint16_t DMA_CCRx_CIRC {DMA_CCR1_CIRC};
  mp_dma_channel->CCR |= DMA_CCRx_CIRC;
}

void STM32_DMA::DisableCircularMode()
{
  constexpr uint16_t DMA_CCRx_CIRC {DMA_CCR1_CIRC};
  mp_dma_channel->CCR &= ~DMA_CCRx_CIRC;
}

bool STM32_DMA::IsBusy()
{
  return true;
}

void STM32_DMA::ClearFlag()
{
}

bool STM32_DMA::IsTransferComplete()
{
  constexpr auto channel_tcif_offset{4};
  constexpr auto channel_number{3};
  
  return DMA1->ISR & (DMA_ISR_TCIF1 << (channel_tcif_offset * channel_number));
  
  return DMA2->ISR & (DMA_ISR_TCIF1 << (channel_tcif_offset * channel_number));
}

bool STM32_DMA::IsHalfTransferComplete()
{
  constexpr auto channel_htif_offset{4};
  constexpr auto channel_number{3};
  
  return DMA1->ISR & (DMA_ISR_TCIF1 << (channel_htif_offset * channel_number));
  
  return DMA2->ISR & (DMA_ISR_TCIF1 << (channel_htif_offset * channel_number));
}