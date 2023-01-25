
#include "Led.h"

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



void Led_vidInit(void)
{
    /*Initial PB5 PE5 to Low output*/
    GPIO_ResetBits(GPIOB, GPIO_Pin_5);
    GPIO_SetBits(GPIOE, GPIO_Pin_5);
}


void Led_vidSetHigh(Led_enuNum LedNum)
{
    if (LED0 == LedNum)    
    {
        GPIO_SetBits(GPIOB, GPIO_Pin_5);
    }
    else if (LED1 == LedNum)
    {
        GPIO_SetBits(GPIOE, GPIO_Pin_5);
    }
    else
    {
        /*Do nothing*/
    }
    
}

void Led_vidSetLow(Led_enuNum LedNum)
{
    if (LED0 == LedNum)    
    {
        GPIO_ResetBits(GPIOB, GPIO_Pin_5);
    }
    else if (LED1 == LedNum)
    {
        GPIO_ResetBits(GPIOE, GPIO_Pin_5);
    }
    else
    {
        /*Do nothing*/
    }
    
}



