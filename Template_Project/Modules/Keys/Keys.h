
#ifndef _KEYS_H
#define _KEYS_H

#include "stm32f10x.h"


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


typedef enum
{
    KEY0_PRESS = 0,
    KEY1_PRESS,
    WKUP_PRESS,
    NO_PRESS,

}Key_enuPress;



#define KEY0 GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)
#define KEY1 GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)
#define WKUP GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)


void Key_vidInit(void);

 

Key_enuPress Key_vidScan(void);


#endif



