
#include "MyDMA.h"


u16 DMA_MemLen = 0;
/*调用例子： MyDMA_Configure(DMA1_Channel4, &USART1->DR, (u32)SendBuff, 1000);*/
/*查芯片手册USART使用DMA通道4,所以是DMA1_Channel4,      要将SendBuff的数据传输到串口发送寄存器USART1->DR,一次传1000个数据*/
void MyDMA_Configure(DMA_Channel_TypeDef *DMA_CHx, u32 cpar, u32 cmar, u16 cndtr)
{
    /*此例子将数据从存储器传输到UART发送寄存器*/

    /*1. 使能DMA时钟,DMA直接连在AHB总线上，所以使用RCC_AHBPeriphClockCmd*/
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); 

    DMA_MemLen = cndtr;/*记录需要传输的数据值*/

    /*2. 初始化DMA*/
    DMA_InitTypeDef DMA_InitStruct;    
    DMA_InitStruct.DMA_PeripheralBaseAddr = cpar;   /*外设寄存器基地址*/
    DMA_InitStruct.DMA_MemoryBaseAddr = cmar;       /*存储器基地址*/
    DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  /*外设数据宽度,8位*/
    DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;          /*存储器数据宽度,8位*/
    DMA_InitStruct.DMA_BufferSize = cndtr;                /*通道传输数据量，8个数据，实际是设置CNDTRx*/
    DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralDST; /*数据传输方向，外设做目标*/
    DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;    /*存储器增量传输模式开启*/
    DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  /*外设增量传输模式关闭*/
    DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;      /*模式：是否循环,不循环*/
    DMA_InitStruct.DMA_Priority = DMA_Priority_High;  /*优先级,高*/
    DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;       /*是否存储器传输存储器，不使用*/
    DMA_Init(DMA_CHx, &DMA_InitStruct);

    /*3. 使能UART1 DMA发送功能*/
    USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);

    /*4. 使能DMA通道，启动传输，使用DMA1控制器的通道1*/
    DMA_Cmd(DMA_CHx, ENABLE);

    /*此时已经开启DMA传输*/
    
    
}



void MyDMA_Enable(DMA_Channel_TypeDef *DMA_CHx)
{
    /*关闭DMA*/
    DMA_Cmd(DMA1_Channel1, DISABLE);
    /*重新设置传输数据量，因为每次传输完毕后CNDTRx的值会递减到0，所以需要重新设置传输数据量*/
    DMA_SetCurrDataCounter(DMA1_Channel1, DMA_MemLen);
    /*重新开启并启动DMA传输*/
    DMA_Cmd(DMA1_Channel1, ENABLE);
}



