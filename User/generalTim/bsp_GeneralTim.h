#ifndef __BSP_ADVANCETIME_H
#define __BSP_ADVANCETIME_H


#include "stm32f10x.h"


/************�߼���ʱ��TIM�������壬ֻ��TIM1��TIM8************/
// ��ʹ�ò�ͬ�Ķ�ʱ����ʱ�򣬶�Ӧ��GPIO�ǲ�һ���ģ����Ҫע��
// ��������ʹ�ø߼����ƶ�ʱ��TIM1

#define            ADVANCE_TIM                   TIM4
#define            ADVANCE_TIM_APBxClock_FUN     RCC_APB1PeriphClockCmd
#define            ADVANCE_TIM_CLK               RCC_APB1Periph_TIM4

// ���벶���ܲ��񵽵���С��Ƶ��Ϊ 72M/{ (ARR+1)*(PSC+1) }
#define            ADVANCE_TIM_PERIOD            (0xFFFF-1)
#define            ADVANCE_TIM_PSC               (0xFFFF-1)

// �ж���غ궨��
#define            ADVANCE_TIM_IRQ               TIM4_IRQn
#define            ADVANCE_TIM_IRQHandler        TIM4_IRQHandler

// TIM1 ���벶��ͨ��1
#define            ADVANCE_TIM_CH1_GPIO_CLK      RCC_APB2Periph_GPIOB
#define            ADVANCE_TIM_CH1_PORT          GPIOB
#define            ADVANCE_TIM_CH1_PIN           GPIO_Pin_6

#define            ADVANCE_TIM_IC1PWM_CHANNEL    TIM_Channel_1
#define            ADVANCE_TIM_IC2PWM_CHANNEL    TIM_Channel_2

/**************************��������********************************/

void ADVANCE_TIM_Init(void);


#endif	/* __BSP_ADVANCETIME_H */


