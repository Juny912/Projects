
#include "Keys.h"

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



void Key_vidInit(void)
{
    /*Enable GPIOE clcok*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOE, ENABLE);
    
    /*Init PE.4 PE.3*/    
    GPIO_InitTypeDef GPIO_InitStrcture;
    GPIO_InitStrcture.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4;        /*PE4*/
    GPIO_InitStrcture.GPIO_Mode = GPIO_Mode_IPU; /*Floating input mode*/

    GPIO_Init(GPIOE, &GPIO_InitStrcture);

    GPIO_InitStrcture.GPIO_Pin = GPIO_Pin_0;        /*PE4*/
    GPIO_InitStrcture.GPIO_Mode = GPIO_Mode_IPD; /*Floating input mode*/

    GPIO_Init(GPIOA, &GPIO_InitStrcture);


}

 


Key_enuPress Key_vidScan(void)
{
    //static u8 cnt = 0;
    Key_enuPress enKeyPress = NO_PRESS;

    if (WKUP||!KEY0||!KEY1)
    {
        /*Delay 30ms avoid flicker*/
        Delay(300000);
        
        if (0 == KEY0)
        {
            enKeyPress = KEY0_PRESS;
        }
        else if (0 == KEY1)
        {
            enKeyPress = KEY1_PRESS;
        }
        else if (1 == WKUP)
        {
            enKeyPress = WKUP_PRESS;
        }
        else if (KEY0&&KEY1&&!WKUP)
        {
            enKeyPress = NO_PRESS;
        }
    }
    return enKeyPress;
}






