#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "FreeRTOS.h"
#include "task.h"
 
/************************************************
 ALIENTEK战舰STM32开发板实验1
 跑马灯实验 
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/

#define START_TASK_PRIO 1       //task优先级
#define START_STK_SIZE  512     //task堆栈大小
TaskHandle_t StartTask_Handler; //task句柄
void start_task(void *pvParameters);        //task主函数

#define LED0_TASK_PRIO  2       //task优先级
#define LED0_STK_SIZE  50     //task堆栈大小
TaskHandle_t Led0Task_Handler; //task句柄
void Led0_task(void *pvParameters);        //task主函数

#define LED1_TASK_PRIO  3       //task优先级
#define LED1_STK_SIZE   50     //task堆栈大小
StackType_t Led1_Stack[LED1_STK_SIZE];
StaticTask_t Led1_TCB;

TaskHandle_t Led1Task_Handler; //task句柄
void Led1_task(void *pvParameters);        //task主函数

#define QUERY_TASK_PRIO  3       //task优先级
#define QUERY_STK_SIZE   256     //task堆栈大小
StackType_t Query_Stack[LED1_STK_SIZE];
StaticTask_t Query_TCB;

TaskHandle_t QueryTask_Handler; //task句柄
void Query_task(void *pvParameters);        //task主函数




void Led0_task(void *pvParameters)
{
    u8 timecnt = 0;
    while(1)
    {
        LED0 = ~LED0;
        vTaskDelay(500);

    }
    
    

}

void Led1_task(void *pvParameters)
{
    while(1)
    {
        LED1 = 0;
        vTaskDelay(200);
        LED1 = 1;
        vTaskDelay(800);
    }

}
UBaseType_t taskPri = 0, taskNum = 0;
eTaskState taskSts;


void Query_task(void *pvParameters)
{
    u32 pulTotalRunTime;
    
    TaskStatus_t *pTaskStat;
    TaskHandle_t taskHandle;
    

    while(1)
    {
        //printf("Get task info\n");
        
        taskPri = uxTaskPriorityGet(Led0Task_Handler);
        taskNum = uxTaskGetNumberOfTasks();
        
        pTaskStat = pvPortMalloc(taskNum*sizeof(TaskStatus_t));
        uxTaskGetSystemState(pTaskStat, taskNum, &pulTotalRunTime);

        taskHandle = xTaskGetHandle("Led0_task");
        taskHandle = xTaskGetIdleTaskHandle();
        vTaskGetInfo(taskHandle, pTaskStat, pdFALSE, taskSts);
        vTaskDelay(2000);
    }

}


void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();
    xTaskCreate((TaskFunction_t)Led0_task, "Led0_task", LED0_STK_SIZE, NULL, LED0_TASK_PRIO, &Led0Task_Handler);
    xTaskCreate((TaskFunction_t)Led1_task, "Led1_task", LED1_STK_SIZE, NULL, LED1_TASK_PRIO, &Led1Task_Handler);
    xTaskCreate((TaskFunction_t)Query_task, "Query_task", QUERY_STK_SIZE, NULL, QUERY_TASK_PRIO, &QueryTask_Handler);
    
    vTaskDelete(StartTask_Handler);
    taskEXIT_CRITICAL();
}


 int main(void)
 {  
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
 
    delay_init();               //延时函数初始化   
    uart_init(115200);          //初始化串口波特率
    LED_Init();

    xTaskCreate((TaskFunction_t)start_task,
                            "Start_task",
                            START_STK_SIZE,
                            NULL,
                            START_TASK_PRIO,
                            &StartTask_Handler);

    vTaskStartScheduler();

 }


 /**
 *****************下面注视的代码是通过调用库函数来实现IO控制的方法*****************************************
int main(void)
{ 
 
    delay_init();         //初始化延时函数
    LED_Init();             //初始化LED端口
    while(1)
    {
            GPIO_ResetBits(GPIOB,GPIO_Pin_5);  //LED0对应引脚GPIOB.5拉低，亮  等同LED0=0;
            GPIO_SetBits(GPIOE,GPIO_Pin_5);   //LED1对应引脚GPIOE.5拉高，灭 等同LED1=1;
            delay_ms(300);             //延时300ms
            GPIO_SetBits(GPIOB,GPIO_Pin_5);    //LED0对应引脚GPIOB.5拉高，灭  等同LED0=1;
            GPIO_ResetBits(GPIOE,GPIO_Pin_5); //LED1对应引脚GPIOE.5拉低，亮 等同LED1=0;
            delay_ms(300);                     //延时300ms
    }
} 
 
 ****************************************************************************************************
 ***/
 

    
/**
*******************下面注释掉的代码是通过 直接操作寄存器 方式实现IO口控制**************************************
int main(void)
{ 
 
    delay_init();         //初始化延时函数
    LED_Init();             //初始化LED端口
    while(1)
    {
     GPIOB->BRR=GPIO_Pin_5;//LED0亮
       GPIOE->BSRR=GPIO_Pin_5;//LED1灭
         delay_ms(300);
     GPIOB->BSRR=GPIO_Pin_5;//LED0灭
       GPIOE->BRR=GPIO_Pin_5;//LED1亮
         delay_ms(300);

     }
 }
**************************************************************************************************
**/

