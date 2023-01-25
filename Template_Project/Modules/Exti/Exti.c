
#include "Exti.h"
#include "Keys.h"
#include "delay.h"
#include "Led.h"
#include "stm32f10x_gpio.h"

void EXTI4_IRQHandler(void)
{
    Delay(300000);
    if (0 == Key_vidScan())
    {
        Led_vidSetLow(LED0);
    }

    EXTI_ClearITPendingBit(EXTI_Line4);
}


void Exti_vidInit(void)
{
    EXTI_InitTypeDef EXTI_InitStruct;
#if 0 /*This part is moved to main function*/
    /*1 Interrupt group configuration*/
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
#endif

    /*2 Enable IO multiplex*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

#if 0 /*This part is moved to main function*/
    /*3.1 Enable GPIOE clcok*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOE, ENABLE);

    
    /*3.2 Init PE.4(KEY0) PE.3(KEY1)*/    
    GPIO_InitTypeDef GPIO_InitStrcture;
    GPIO_InitStrcture.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4;        /*PE4*/
    GPIO_InitStrcture.GPIO_Mode = GPIO_Mode_IPU; /*Floating input mode*/
    GPIO_Init(GPIOE, &GPIO_InitStrcture);

    GPIO_InitStrcture.GPIO_Pin = GPIO_Pin_0;        /*PE4*/
    GPIO_InitStrcture.GPIO_Mode = GPIO_Mode_IPD; /*Floating input mode*/
    GPIO_Init(GPIOA, &GPIO_InitStrcture);
#endif

    /*4 Configure PE4(KEY0) to EXTI line*/
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource4);

    /*5.1 Initial EXTI line4*/
    EXTI_InitStruct.EXTI_Line = EXTI_Line4;
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_Init(&EXTI_InitStruct);

    /*5.2 Configure EXTI4 Interrupt (Preemption/SubPriority priority)*/
    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannel = EXTI4_IRQn;    
    NVIC_Init(&NVIC_InitStruct);


}



