#include "stm32f10x.h"
#include "stm32f10x_iwdg.h"
#include "Led.h"
#include "Keys.h"
#include "MyUsart.h"
#include "Exti.h"
#include "delay.h"
#include "AT24C0x.h"
#include "Hc05.h"
/************************************************
 ALIENTEK 战舰STM32F103开发板实验0
 工程模板
 注意，这是手册中的新建工程章节使用的main文件 
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/




void IWDG_vidInitialization(void)
{
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
    IWDG_SetPrescaler(IWDG_Prescaler_64);
    IWDG_SetReload(625);    
    IWDG_ReloadCounter();
    IWDG_Enable();
}



void SystemTickConfig(u8 u8ClkSource, u32 u32Ticks)
{
    SysTick_CLKSourceConfig(u8ClkSource); /*HCLK = 72MHz*/
    SysTick_Config(u32Ticks);/*SysTick interrupt : SysTick_Handler()*/
}


 void Delay(u32 count)
 {
   u32 i=0;
   for(;i<count;i++);
 } 


void Timer_vidInit(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    TIM_TimeBaseInitStruct.TIM_Period = 4999;
    TIM_TimeBaseInitStruct.TIM_Prescaler = 7199;
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);

    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

    TIM_Cmd(TIM3, ENABLE);       
    
}


void Timer_PWMInit(void)
{
    /*Enbale Timer3 clock*/
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);    

    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    TIM_TimeBaseInitStruct.TIM_Prescaler = 3599;  /*Timer clock = 72MHz/(3599+1)= 20000Hz*/
    TIM_TimeBaseInitStruct.TIM_Period = 50;       /*PWM frequency = 20000Hz/400Hz = 50*/    
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);

    TIM_OCInitTypeDef TIM_OCInitStruct;
    TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStruct.TIM_OCNPolarity = TIM_OCPolarity_Low;
    TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OC2Init(TIM3, &TIM_OCInitStruct);

    TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
    TIM_Cmd(TIM3, ENABLE);
}

u8 ReadBuff[10] = {0};






 int main(void)
 {  

    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE; 
    NVIC_Init(&NVIC_InitStruct);
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    /*1.  GPIO Init*/
    /*1.1 Enable GPIOA|GPIOB|GPIOE Clock*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOE|RCC_APB2Periph_USART1|RCC_APB2Periph_AFIO, ENABLE);
    /*Enable  clcok*/

    /*1.2 UART GPIO config*/
    GPIO_InitTypeDef GPIO_InitStrcture;
    GPIO_InitStrcture.GPIO_Pin = GPIO_Pin_9;         /*PA9 UART TXD*/
    GPIO_InitStrcture.GPIO_Speed = GPIO_Speed_10MHz; 
    GPIO_InitStrcture.GPIO_Mode = GPIO_Mode_AF_PP;  /*Push-pull output mode*/
    GPIO_Init(GPIOA, &GPIO_InitStrcture);
    GPIO_InitStrcture.GPIO_Pin = GPIO_Pin_10;         /*PA10 UART RXD*/
    GPIO_InitStrcture.GPIO_Speed = GPIO_Speed_10MHz; 
    GPIO_InitStrcture.GPIO_Mode = GPIO_Mode_IN_FLOATING;  /*Push-pull output mode*/
    GPIO_Init(GPIOA, &GPIO_InitStrcture);


    /*1.3 KEY GPIO config*/    
    GPIO_InitStrcture.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4; /*PE4-KEY0, PE3-KEY1*/
    GPIO_InitStrcture.GPIO_Mode = GPIO_Mode_IPU;        /*Floating input mode*/
    GPIO_Init(GPIOE, &GPIO_InitStrcture);
    GPIO_InitStrcture.GPIO_Pin = GPIO_Pin_0;            /*PA0-WK_UP*/
    GPIO_InitStrcture.GPIO_Mode = GPIO_Mode_IPD;        /*Floating input mode*/
    GPIO_Init(GPIOA, &GPIO_InitStrcture);


    /*1.4 HC05 GPIO Config*/
    GPIO_InitStrcture.GPIO_Pin = GPIO_Pin_0;            /*PB0 to HC05 STATE Port(LED)*/
    GPIO_InitStrcture.GPIO_Speed = GPIO_Speed_10MHz; 
    GPIO_InitStrcture.GPIO_Mode = GPIO_Mode_IN_FLOATING;/*Floating input mode*/
    GPIO_Init(GPIOB, &GPIO_InitStrcture);
    GPIO_InitStrcture.GPIO_Pin = GPIO_Pin_1;            /*PB1 to HC05 EN Port(KEY)*/
    GPIO_InitStrcture.GPIO_Speed = GPIO_Speed_10MHz; 
    GPIO_InitStrcture.GPIO_Mode = GPIO_Mode_AF_PP;      /*Push-pull output mode*/
    GPIO_Init(GPIOB, &GPIO_InitStrcture);


    /*1.5 LED GPIO Config*/    
    GPIO_InitStrcture.GPIO_Pin = GPIO_Pin_5;            /*PB5-LED0*/
    GPIO_InitStrcture.GPIO_Mode = GPIO_Mode_Out_PP;     /*Output mode*/
    GPIO_InitStrcture.GPIO_Speed = GPIO_Speed_10MHz;   
    GPIO_Init(GPIOB, &GPIO_InitStrcture);

    GPIO_InitStrcture.GPIO_Pin = GPIO_Pin_5;            /*PB5-LED1*/
    GPIO_InitStrcture.GPIO_Mode = GPIO_Mode_Out_PP;     /*Output mode*/
    GPIO_InitStrcture.GPIO_Speed = GPIO_Speed_10MHz; 
    GPIO_Init(GPIOE, &GPIO_InitStrcture);

    
    /*1.6 MyIIC GPIO config*/
    GPIO_InitStrcture.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2; /*PA1-SDA, PA2-SCL*/
    GPIO_InitStrcture.GPIO_Mode = GPIO_Mode_Out_PP;     /*Output mode*/
    GPIO_InitStrcture.GPIO_Speed = GPIO_Speed_10MHz;   
    GPIO_Init(GPIOA, &GPIO_InitStrcture);

    /*1.6 Timer3 PWM output GPIO config*/
    /*Partial remap Timer3*/
    GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE);
    GPIO_InitStrcture.GPIO_Pin = GPIO_Pin_7;        /*PC7, TIM3_CH2 FullRemap to PC7*/
    GPIO_InitStrcture.GPIO_Mode = GPIO_Mode_AF_PP;  /*Multiplex output*/
    GPIO_InitStrcture.GPIO_Speed = GPIO_Speed_10MHz;   
    GPIO_Init(GPIOC, &GPIO_InitStrcture);


    
    Key_enuPress enKeyPress;
    delay_init();

    //Timer_vidInit(); 
    /*WDG initialization*/
    //IWDG_vidInitialization();
    //SystemTickConfig(SysTick_CLKSource_HCLK_Div8,72000000);
    
    
    //Timer_PWMInit();
    
    //delay_ms(500);
    //Led_vidInit();
    //Key_vidInit();

    MyUsart_vidInit();

    Hc05_Init();

    //Hc05_SendCmd(HC05_SET_AT);
    
    //Exti_vidInit();
    
    //Timer_PWMInit();
    //TIM_SetCompare2(TIM3, 40);


    AT24C0X_Init();

    AT24C0X_WriteOneByte(EEPROM_ADDR, 0x12);

    ReadBuff[0] = AT24C0X_ReadOneByte(EEPROM_ADDR);
    
    while(1)
    {

#if 0

        enKeyPress = Key_vidScan();

        switch (enKeyPress)
        {
            case KEY0_PRESS:
                Led_vidSetLow(LED0);
                //USART_SendData(USART1, 0x1);
                //Led_vidSetLow(LED1);
                IWDG_ReloadCounter();
                break;
            case KEY1_PRESS:
                //Led_vidSetLow(LED1);
                Led_vidSetLow(LED1);
                //USART_SendData(USART1, 0x2);
                break;
            case WKUP_PRESS:
                Led_vidSetHigh(LED0);
                Led_vidSetHigh(LED1);
                break;
        }
        
#endif        
        //Hc05_SendCmd("ABCDEF");
        while(!USART_GetFlagStatus(USART1, USART_FLAG_TC));
        USART_SendData(USART1, 8); 
        delay_ms(1000);

    }
    







  
 }
