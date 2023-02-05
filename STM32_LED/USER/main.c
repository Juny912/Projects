#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "FreeRTOS.h"
#include "task.h"
 
/************************************************
 ALIENTEKս��STM32������ʵ��1
 �����ʵ�� 
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/

#define START_TASK_PRIO 1       //task���ȼ�
#define START_STK_SIZE  512     //task��ջ��С
TaskHandle_t StartTask_Handler; //task���
void start_task(void *pvParameters);        //task������

#define LED0_TASK_PRIO  2       //task���ȼ�
#define LED0_STK_SIZE  50     //task��ջ��С
TaskHandle_t Led0Task_Handler; //task���
void Led0_task(void *pvParameters);        //task������

#define LED1_TASK_PRIO  3       //task���ȼ�
#define LED1_STK_SIZE   50     //task��ջ��С
StackType_t Led1_Stack[LED1_STK_SIZE];
StaticTask_t Led1_TCB;

TaskHandle_t Led1Task_Handler; //task���
void Led1_task(void *pvParameters);        //task������

#define QUERY_TASK_PRIO  3       //task���ȼ�
#define QUERY_STK_SIZE   256     //task��ջ��С
StackType_t Query_Stack[LED1_STK_SIZE];
StaticTask_t Query_TCB;

TaskHandle_t QueryTask_Handler; //task���
void Query_task(void *pvParameters);        //task������




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
 
    delay_init();               //��ʱ������ʼ��   
    uart_init(115200);          //��ʼ�����ڲ�����
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
 *****************����ע�ӵĴ�����ͨ�����ÿ⺯����ʵ��IO���Ƶķ���*****************************************
int main(void)
{ 
 
    delay_init();         //��ʼ����ʱ����
    LED_Init();             //��ʼ��LED�˿�
    while(1)
    {
            GPIO_ResetBits(GPIOB,GPIO_Pin_5);  //LED0��Ӧ����GPIOB.5���ͣ���  ��ͬLED0=0;
            GPIO_SetBits(GPIOE,GPIO_Pin_5);   //LED1��Ӧ����GPIOE.5���ߣ��� ��ͬLED1=1;
            delay_ms(300);             //��ʱ300ms
            GPIO_SetBits(GPIOB,GPIO_Pin_5);    //LED0��Ӧ����GPIOB.5���ߣ���  ��ͬLED0=1;
            GPIO_ResetBits(GPIOE,GPIO_Pin_5); //LED1��Ӧ����GPIOE.5���ͣ��� ��ͬLED1=0;
            delay_ms(300);                     //��ʱ300ms
    }
} 
 
 ****************************************************************************************************
 ***/
 

    
/**
*******************����ע�͵��Ĵ�����ͨ�� ֱ�Ӳ����Ĵ��� ��ʽʵ��IO�ڿ���**************************************
int main(void)
{ 
 
    delay_init();         //��ʼ����ʱ����
    LED_Init();             //��ʼ��LED�˿�
    while(1)
    {
     GPIOB->BRR=GPIO_Pin_5;//LED0��
       GPIOE->BSRR=GPIO_Pin_5;//LED1��
         delay_ms(300);
     GPIOB->BSRR=GPIO_Pin_5;//LED0��
       GPIOE->BRR=GPIO_Pin_5;//LED1��
         delay_ms(300);

     }
 }
**************************************************************************************************
**/

