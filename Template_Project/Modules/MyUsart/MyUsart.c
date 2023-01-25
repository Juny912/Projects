
#include "MyUsart.h"

u8 Uart_SendBuff[10] = {0};
u8 Uart_RecvBuff[30] = {0};
u8 iTest=0;

void USART1_IRQHandler(void)
{
    static u8 res;
    res = USART_GetITStatus(USART1, USART_IT_RXNE);
    if (res)
    {
        Uart_RecvBuff[iTest] = USART_ReceiveData(USART1);
        iTest++;
        //USART_SendData(USART1, Ret);
    }

    if (iTest == 99)
    {
        iTest = 0;
    }
    
}



void MyUsart_vidInit(void)
{

    /*Init USART1*/
    USART_InitTypeDef USART_InitStruct;
    USART_InitStruct.USART_BaudRate = 9600;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStruct.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
    USART_InitStruct.USART_Parity = USART_Parity_No;
    USART_InitStruct.USART_StopBits = USART_StopBits_1;
    USART_InitStruct.USART_WordLength = USART_WordLength_8b; /*No parity bit use, so it's 8Bit*/
    USART_Init(USART1, &USART_InitStruct);

    /*Enable USART1*/
    USART_Cmd(USART1,ENABLE);

    /*Enable USART1 RX not empty interrupt*/
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

    /*Configure USART1 Interrupt (Preemption/SubPriority priority)*/
    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;    
    NVIC_Init(&NVIC_InitStruct);

    //USART_SendData(USART1, 0xfa);
}


void MyUart_SendBuffer(char *buffer, u8 len)
{
    u8 temp=0;
    for (temp=0;temp<len;temp++)
    {
         while(!USART_GetFlagStatus(USART1, USART_FLAG_TC));
         USART_SendData(USART1, buffer[temp]);        
    }
}




