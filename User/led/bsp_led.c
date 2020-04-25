/**
  ******************************************************************************
  * @file    bsp_led.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   ledӦ�ú����ӿ�
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� F103-MINI STM32 ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "bsp_led.h"   

 /**
  * @brief  ��ʼ������LED��IO
  * @param  ��
  * @retval ��
  */
void LED_GPIO_Config(void)
{		
		/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
		GPIO_InitTypeDef GPIO_InitStructure;
	
		RCC_APB2PeriphClockCmd( LED1_GPIO_CLK, ENABLE);
		GPIO_InitStructure.GPIO_Pin = LED1_GPIO_PIN | LED2_GPIO_PIN | LED3_GPIO_PIN | LED4_GPIO_PIN;	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
		GPIO_Init(LED1_GPIO_PORT, &GPIO_InitStructure);	
		GPIO_ResetBits(LED1_GPIO_PORT, LED1_GPIO_PIN);
		GPIO_ResetBits(LED2_GPIO_PORT, LED2_GPIO_PIN);
		GPIO_ResetBits(LED3_GPIO_PORT, LED3_GPIO_PIN);
	
		/*RCC_APB2PeriphClockCmd( LED2_GPIO_CLK, ENABLE);
		GPIO_InitStructure.GPIO_Pin = LED2_GPIO_PIN;	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
		GPIO_Init(LED2_GPIO_PORT, &GPIO_InitStructure);	
		GPIO_ResetBits(LED2_GPIO_PORT, LED2_GPIO_PIN);
			
	  RCC_APB2PeriphClockCmd( LED3_GPIO_CLK, ENABLE);
		GPIO_InitStructure.GPIO_Pin = LED3_GPIO_PIN;	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
		GPIO_Init(LED3_GPIO_PORT, &GPIO_InitStructure);	
		GPIO_ResetBits(LED3_GPIO_PORT, LED3_GPIO_PIN);
				
		RCC_APB2PeriphClockCmd( LED4_GPIO_CLK, ENABLE);
		GPIO_InitStructure.GPIO_Pin = LED4_GPIO_PIN;	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
		GPIO_Init(LED4_GPIO_PORT, &GPIO_InitStructure);	
		GPIO_ResetBits(LED4_GPIO_PORT, LED4_GPIO_PIN);*/
				
		RCC_APB2PeriphClockCmd( LED5_GPIO_CLK, ENABLE);
		GPIO_InitStructure.GPIO_Pin = LED5_GPIO_PIN | LED6_GPIO_PIN;	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
		GPIO_Init(LED5_GPIO_PORT, &GPIO_InitStructure);	
		GPIO_ResetBits(LED5_GPIO_PORT, LED5_GPIO_PIN);
		GPIO_ResetBits(LED6_GPIO_PORT, LED6_GPIO_PIN);
				
		/*RCC_APB2PeriphClockCmd( LED6_GPIO_CLK, ENABLE);
		GPIO_InitStructure.GPIO_Pin = LED6_GPIO_PIN;	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
		GPIO_Init(LED6_GPIO_PORT, &GPIO_InitStructure);	
		GPIO_ResetBits(LED6_GPIO_PORT, LED6_GPIO_PIN);*/
}
		
void aaaFK_GPIO_Config(void)
{
	  GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);    
    /* Disable JLink, enable SW */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE); 
	  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);   
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
    /* Push-pill output, it can be other output types */
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
    /* Push-pill output, it can be other output types */
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_Init(GPIOA, &GPIO_InitStructure);

}

/*********************************************END OF FILE**********************/
