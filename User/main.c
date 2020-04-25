/**
  *********************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   FreeRTOS V9.0.0  + STM32 �̼�������
  *********************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� STM32 ȫϵ�п����� 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  **********************************************************************
  */ 
 
/*
*************************************************************************
*                             ������ͷ�ļ�
*************************************************************************
*/ 
#include <stdlib.h>
#include <stdio.h>
/* FreeRTOSͷ�ļ� */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "event_groups.h"
/* ������Ӳ��bspͷ�ļ� */
#include "bsp_led.h"
#include "bsp_key.h"
#include "delay.h"
#include "bsp_usart1.h"
#include "./usart/bsp_usart2.h"
#include "tasks.h"
#include "bsp_adc.h"
#include "./led/bsp_led.h" 
#include "./i2c/bsp_i2c_ee.h"
#include "bsp_GeneralTim.h" 


extern uint8_t SendBuff1[SENDBUFF1_SIZE];
extern uint8_t SendBuff2[SENDBUFF2_SIZE];
extern __IO uint16_t ADC_ConvertedValue[NOFCHANEL];  //ADCģ��ת����ѹ
extern float ADC_ConvertedValueLocal1;     // �ֲ����������ڱ���ת�������ĵ�ѹֵ 	 
extern float ADC_ConvertedValueLocal2;     // �ֲ����������ڱ���ת�������ĵ�ѹֵ 	
extern uint8_t zhangli_qidong;
extern uint8_t yunxing_zhuangtai;

/**************************** ������ ********************************/
/* 
 * ��������һ��ָ�룬����ָ��һ�����񣬵����񴴽���֮�����;�����һ��������
 * �Ժ�����Ҫ��������������Ҫͨ�������������������������������Լ�����ô
 * ����������ΪNULL��
 */
static TaskHandle_t AppTaskCreate_Handle = NULL;/* ���������� */
static TaskHandle_t Jiting_Task_Handle = NULL;/* LED������ */
static TaskHandle_t Main_Task_Handle = NULL;/* KEY������ */
static TaskHandle_t ADC_Task_Handle = NULL;/* KEY������ */
static TaskHandle_t Diandong_Task_Handle = NULL;/* KEY������ */
static TaskHandle_t Kaiguan_Task_Handle = NULL;/* KEY������ */
static TaskHandle_t Zhuangtai_Task_Handle = NULL;/* KEY������ */
extern TimerHandle_t Swtmr1_Handle;   /* �����ʱ����� */

/********************************** �ں˶����� *********************************/
/*
 * �ź�������Ϣ���У��¼���־�飬�����ʱ����Щ�������ں˵Ķ���Ҫ��ʹ����Щ�ں�
 * ���󣬱����ȴ����������ɹ�֮��᷵��һ����Ӧ�ľ����ʵ���Ͼ���һ��ָ�룬������
 * �ǾͿ���ͨ��������������Щ�ں˶���
 *
 * �ں˶���˵���˾���һ��ȫ�ֵ����ݽṹ��ͨ����Щ���ݽṹ���ǿ���ʵ��������ͨ�ţ�
 * �������¼�ͬ���ȸ��ֹ��ܡ�������Щ���ܵ�ʵ��������ͨ��������Щ�ں˶���ĺ���
 * ����ɵ�
 * 
 */
QueueHandle_t Test_Queue =NULL;

/******************************* ȫ�ֱ������� ************************************/
/*
 * ��������дӦ�ó����ʱ�򣬿�����Ҫ�õ�һЩȫ�ֱ�����
 */

/******************************* �궨�� ************************************/
/*
 * ��������дӦ�ó����ʱ�򣬿�����Ҫ�õ�һЩ�궨�塣
 */
#define  QUEUE_LEN    4   /* ���еĳ��ȣ����ɰ������ٸ���Ϣ */
#define  QUEUE_SIZE   4   /* ������ÿ����Ϣ��С���ֽڣ� */


/*
*************************************************************************
*                             ��������
*************************************************************************
*/
static void AppTaskCreate(void);/* ���ڴ������� */

static void Jiting_Task(void* pvParameters);/* Test_Task����ʵ�� */
static void Main_Task(void* pvParameters);/* KEY_Task����ʵ�� */
static void ADC_Task(void* pvParameters);/* KEY_Task����ʵ�� */
static void Diandong_Task(void* pvParameters);/* KEY_Task����ʵ�� */
static void Kaiguan_Task(void* pvParameters);/* KEY_Task����ʵ�� */
static void Zhuangtai_Task(void* pvParameters);/* KEY_Task����ʵ�� */
static void Swtmr1_Callback(void* parameter);

static void BSP_Init(void);/* ���ڳ�ʼ�����������Դ */

/*****************************************************************
  * @brief  ������
  * @param  ��
  * @retval ��
  * @note   ��һ����������Ӳ����ʼ�� 
            �ڶ���������APPӦ������
            ������������FreeRTOS����ʼ���������
  ****************************************************************/
int main(void)
{	
  BaseType_t xReturn = pdPASS;/* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */
	
	SystemInit();           //ϵͳ��ʼ��
	
  /* ������Ӳ����ʼ�� */
  BSP_Init();  
 
  /* ����Test_Queue */
  Test_Queue = xQueueCreate((UBaseType_t ) QUEUE_LEN,/* ��Ϣ���еĳ��� */
                            (UBaseType_t ) QUEUE_SIZE);/* ��Ϣ�Ĵ�С */
  
  
   /* ����AppTaskCreate���� */
  xReturn = xTaskCreate((TaskFunction_t )AppTaskCreate,  /* ������ں��� */
                        (const char*    )"AppTaskCreate",/* �������� */
                        (uint16_t       )512,  /* ����ջ��С */
                        (void*          )NULL,/* ������ں������� */
                        (UBaseType_t    )1, /* ��������ȼ� */
                        (TaskHandle_t*  )&AppTaskCreate_Handle);/* ������ƿ�ָ�� */ 
  /* ����������� */           
  if(pdPASS == xReturn)
    vTaskStartScheduler();   /* �������񣬿������� */
  else
			NVIC_SysReset();
    return -1;  
  
  while(1);   /* ��������ִ�е����� */    
}


/***********************************************************************
  * @ ������  �� AppTaskCreate
  * @ ����˵���� Ϊ�˷���������е����񴴽����������������������
  * @ ����    �� ��  
  * @ ����ֵ  �� ��
  **********************************************************************/
static void AppTaskCreate(void)
{
  BaseType_t xReturn = pdPASS;/* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */
  
  taskENTER_CRITICAL();           //�����ٽ���
  

  xReturn = xTaskCreate((TaskFunction_t )Main_Task,  /* ������ں��� */
                        (const char*    )"Main_Task",/* �������� */
                        (uint16_t       )512,  /* ����ջ��С */
                        (void*          )NULL,/* ������ں������� */
                        (UBaseType_t    )4, /* ��������ȼ� */
                        (TaskHandle_t*  )&Main_Task_Handle);/* ������ƿ�ָ�� */ 
												
									
	xReturn = xTaskCreate((TaskFunction_t )Jiting_Task,  /* ������ں��� */
                        (const char*    )"Jiting_Task",/* �������� */
                        (uint16_t       )512,  /* ����ջ��С */
                        (void*          )NULL,/* ������ں������� */
                        (UBaseType_t    )5, /* ��������ȼ� */
                        (TaskHandle_t*  )&Jiting_Task_Handle);/* ������ƿ�ָ�� */ 
												
	xReturn = xTaskCreate((TaskFunction_t )ADC_Task,  /* ������ں��� */
                        (const char*    )"ADC_Task",/* �������� */
                        (uint16_t       )512,  /* ����ջ��С */
                        (void*          )NULL,/* ������ں������� */
                        (UBaseType_t    )2, /* ��������ȼ� */
                        (TaskHandle_t*  )&ADC_Task_Handle);/* ������ƿ�ָ�� */ 
												
	xReturn = xTaskCreate((TaskFunction_t )Diandong_Task,  /* ������ں��� */
                        (const char*    )"Diandong_Task",/* �������� */
                        (uint16_t       )512,  /* ����ջ��С */
                        (void*          )NULL,/* ������ں������� */
                        (UBaseType_t    )2, /* ��������ȼ� */
                        (TaskHandle_t*  )&Diandong_Task_Handle);/* ������ƿ�ָ�� */ 
	
	xReturn = xTaskCreate((TaskFunction_t )Zhuangtai_Task,  /* ������ں��� */
                        (const char*    )"Zhuangtai_Task",/* �������� */
                        (uint16_t       )256,  /* ����ջ��С */
                        (void*          )NULL,/* ������ں������� */
                        (UBaseType_t    )2, /* ��������ȼ� */
                        (TaskHandle_t*  )&Zhuangtai_Task_Handle);/* ������ƿ�ָ�� */ 
	
	xReturn = xTaskCreate((TaskFunction_t )Kaiguan_Task,  /* ������ں��� */
                        (const char*    )"Kaiguan_Task",/* �������� */
                        (uint16_t       )256,  /* ����ջ��С */
                        (void*          )NULL,/* ������ں������� */
                        (UBaseType_t    )2, /* ��������ȼ� */
                        (TaskHandle_t*  )&Kaiguan_Task_Handle);/* ������ƿ�ָ�� */ 
												
												
	
	Swtmr1_Handle=xTimerCreate((const char*			)"OneShotTimer",
                             (TickType_t			)120000,/* ��ʱ������ 1s */
                             (UBaseType_t			)pdFALSE,/* ����ģʽ */
                             (void*					  )1,/* Ϊÿ����ʱ������һ��������ΨһID */
                             (TimerCallbackFunction_t)Swtmr1_Callback); 
 
  vTaskDelete(AppTaskCreate_Handle); //ɾ��AppTaskCreate����
  
  taskEXIT_CRITICAL();            //�˳��ٽ���
}



static void Jiting_Task(void* parameter)
{
  while(1)						//ѭ��
	{
		jiting_task();
		vTaskDelay(50);			//��ʱ20��tic
	}
}



static void ADC_Task(void* parameter) //ģ����
{
   while(1)
	 {
		wendu_task();
		if(zhangli_qidong ==1)
		{
			zhangli_task();
		}
		vTaskDelay(1000);
	 }
}

static void Kaiguan_Task(void* parameter) //ģ����
{
   while(1)
	 {
		kaiguan_task();
		vTaskDelay(50);
	 }
}

static void Diandong_Task(void* parameter) //ģ����
{
   while(1)
	 {
		diandong_renwu();
		vTaskDelay(50);
	 }
}

static void Main_Task(void* parameter) //������
{
   while(1)
	 {
		 zhu_renwu();
		 vTaskDelay(50);
	 }
}

static void Zhuangtai_Task(void* parameter)
{
  while(1)						//ѭ��
	{
		zhuangtai_renwu();
		vTaskDelay(50);			//��ʱ20��tic
	}
}


/***********************************************************************
  * @ ������  �� Swtmr2_Callback
  * @ ����˵���� �����ʱ��2 �ص���������ӡ�ص�������Ϣ&��ǰϵͳʱ��
  *              �����ʱ���벻Ҫ��������������Ҳ��Ҫ������ѭ����Ӧ������
  * @ ����    �� ��  
  * @ ����ֵ  �� ��
  **********************************************************************/
static void Swtmr1_Callback(void* parameter)
{	
	yunxing_zhuangtai=1;
}

/***********************************************************************
  * @ ������  �� BSP_Init
  * @ ����˵���� �弶�����ʼ�������а����ϵĳ�ʼ�����ɷ��������������
  * @ ����    ��   
  * @ ����ֵ  �� ��
  *********************************************************************/
static void BSP_Init(void)
{
	/*
	 * STM32�ж����ȼ�����Ϊ4����4bit��������ʾ��ռ���ȼ�����ΧΪ��0~15
	 * ���ȼ�����ֻ��Ҫ����һ�μ��ɣ��Ժ������������������Ҫ�õ��жϣ�
	 * ��ͳһ��������ȼ����飬ǧ��Ҫ�ٷ��飬�мɡ�
	 */
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );
	
	
	delay_init(72);	     //��ʱ��ʼ��
	
	/* LED ����� ��ʼ�� */
	LED_GPIO_Config();
  
  /* �������������ʼ��	*/
 Key_GPIO_Config();
	FK_GPIO_Config();
	
	/* ���ڳ�ʼ��,�˳�����	*/
	USART1_Config(); 
	  /* ����ʹ��DMAģʽ */
		/* ���ڳ�ʼ��,�˳�����	*/
   USART2_Config(); 
	  /* ����ʹ��DMAģʽ */
	//printf("//////////////////////////////// \r\n");
	
	ADVANCE_TIM_Init();
	
	/* ģ������ʼ��	*/
	ADCx_Init();

}

/********************************END OF FILE****************************/
