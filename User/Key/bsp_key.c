/**
  ******************************************************************************
  * @file    bsp_key.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   ����Ӧ��bsp��ɨ��ģʽ��
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:���� F103-MINI STM32 ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 
  
#include "bsp_key.h"  
#include "FreeRTOS.h"
#include "task.h"


/**
  * @brief  ���ð����õ���I/O��
  * @param  ��
  * @retval ��
  */
void Key_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
  
  RCC_APB2PeriphClockCmd(KEY1_GPIO_CLK,ENABLE); //���������˿�PB��ʱ��
	GPIO_InitStructure.GPIO_Pin = KEY1_GPIO_PIN; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //�˿�����ΪX������
	GPIO_Init(KEY1_GPIO_PORT, &GPIO_InitStructure);	//��ʼ���˿�

	GPIO_InitStructure.GPIO_Pin = KEY2_GPIO_PIN; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //�˿�����Ϊx������
	GPIO_Init(KEY2_GPIO_PORT, &GPIO_InitStructure);	//��ʼ���˿�
	
	GPIO_InitStructure.GPIO_Pin = KEY3_GPIO_PIN; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //�˿�����Ϊx������
	GPIO_Init(KEY2_GPIO_PORT, &GPIO_InitStructure);	//��ʼ���˿�
	
	GPIO_InitStructure.GPIO_Pin = KEY4_GPIO_PIN; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //�˿�����Ϊx������
	GPIO_Init(KEY2_GPIO_PORT, &GPIO_InitStructure);	//��ʼ���˿�

	GPIO_InitStructure.GPIO_Pin = KEY5_GPIO_PIN; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //�˿�����Ϊx������
	GPIO_Init(KEY2_GPIO_PORT, &GPIO_InitStructure);	//��ʼ���˿�
	
	GPIO_InitStructure.GPIO_Pin = KEY13_GPIO_PIN; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //�˿�����Ϊx������
	GPIO_Init(KEY13_GPIO_PORT, &GPIO_InitStructure);	//��ʼ���˿�
	
	RCC_APB2PeriphClockCmd(KEY6_GPIO_CLK,ENABLE); 
	GPIO_InitStructure.GPIO_Pin = KEY6_GPIO_PIN |KEY7_GPIO_PIN|KEY8_GPIO_PIN|KEY9_GPIO_PIN; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //�˿�����Ϊx������
	GPIO_Init(KEY6_GPIO_PORT, &GPIO_InitStructure);	//��ʼ���˿�
	
	/*GPIO_InitStructure.GPIO_Pin = KEY7_GPIO_PIN; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //�˿�����Ϊx������
	GPIO_Init(KEY7_GPIO_PORT, &GPIO_InitStructure);	//��ʼ���˿�
	
	GPIO_InitStructure.GPIO_Pin = KEY8_GPIO_PIN; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //�˿�����Ϊx������
	GPIO_Init(KEY8_GPIO_PORT, &GPIO_InitStructure);	//��ʼ���˿�
	
	GPIO_InitStructure.GPIO_Pin = KEY9_GPIO_PIN; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //�˿�����Ϊx������
	GPIO_Init(KEY9_GPIO_PORT, &GPIO_InitStructure);	//��ʼ���˿�
	
	
	GPIO_InitStructure.GPIO_Pin = KEY13_GPIO_PIN; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //�˿�����Ϊx������
	GPIO_Init(KEY13_GPIO_PORT, &GPIO_InitStructure);	//��ʼ���˿�*/

}

void FK_GPIO_Config(void)
{
	  GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);    
    /* Disable JLink, enable SW */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE); 
	  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);   
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;   
    /* Push-pill output, it can be other output types */
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;   
    /* Push-pill output, it can be other output types */
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
    GPIO_Init(GPIOA, &GPIO_InitStructure);

}
	


 /*
 * ��������Key_Scan
 * ����  ������Ƿ��а�������
 * ����  ��GPIOx��x ������ A��B��C��D���� E
 *		     GPIO_Pin������ȡ�Ķ˿�λ 	
 * ���  ��KEY_OFF(û���°���)��KEY_ON�����°�����
 */
uint8_t Key_Scan_Low(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin)
{			
	/*����Ƿ��а������� */
	if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON )  
	{	 
		/*�ȴ������ͷ� */
		vTaskDelay(100);
		if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON )
		{
			while(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON);   
			return 	KEY_ON;
		}
		else{
		return KEY_OFF;}
	}
	else{
		return KEY_OFF;}
}

uint8_t Key_Scan_High(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin)
{			
	/*����Ƿ��а������� */
	if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == 1 )  
	{	 
		/*�ȴ������ͷ� */
		vTaskDelay(100);
		if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == 1 )
		{
			//while(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == 1);   
			return 	1;
		}
		else{
		return 0;}
	}
	else{
		return 0;}
}

uint8_t Key_Scan_High1(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin)
{			
	/*����Ƿ��а������� */
	if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == 1 )  
	{	 
		/*�ȴ������ͷ� */
		vTaskDelay(100);
		if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == 1 )
		{
			while(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == 1);   
			return 	1;
		}
		else{
		return 0;}
	}
	else{
		return 0;}
}


/*********************************************END OF FILE**********************/
