
#include "bsp_usart2.h"
#include <string.h>
#include "./data_process/data_process.h"

uint8_t SendBuff2[SENDBUFF2_SIZE];
uint8_t Usart2_Rx_Buf[USART2_RX_BUFF_SIZE];

static void USART2_DMA_Rx_Config(void);
static void USART2_DMA_Tx_Config(void);
void USART2_Receive_DataPack(void);

static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Ƕ�������жϿ�������ѡ�� */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
  
  /* ����USARTΪ�ж�Դ */
  NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART2_IRQ;
  /* �������ȼ�*/
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  /* �����ȼ� */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  /* ʹ���ж� */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  /* ��ʼ������NVIC */
  NVIC_Init(&NVIC_InitStructure);
}


/**
  * @brief  USART GPIO ����,������������
  * @param  ��
  * @retval ��
  */
void USART2_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	// �򿪴���GPIO��ʱ��
	DEBUG_USART2_GPIO_APBxClkCmd(DEBUG_USART2_GPIO_CLK, ENABLE);
	
	// �򿪴��������ʱ��
	DEBUG_USART2_APBxClkCmd(DEBUG_USART2_CLK, ENABLE);

	// ��USART Tx��GPIO����Ϊ���츴��ģʽ
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART2_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DEBUG_USART2_TX_GPIO_PORT, &GPIO_InitStructure);

  // ��USART Rx��GPIO����Ϊ��������ģʽ
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART2_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(DEBUG_USART2_RX_GPIO_PORT, &GPIO_InitStructure);
	
	// ���ô��ڵĹ�������
	// ���ò�����
	USART_InitStructure.USART_BaudRate = DEBUG_USART2_BAUDRATE;
	// ���� �������ֳ�
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// ����ֹͣλ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	// ����У��λ
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	// ����Ӳ��������
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	// ���ù���ģʽ���շ�һ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// ��ɴ��ڵĳ�ʼ������
	USART_Init(DEBUG_USART2, &USART_InitStructure);	
	// �����ж����ȼ�����
	NVIC_Configuration();
	
	
	USART_ITConfig(DEBUG_USART2, USART_IT_IDLE, ENABLE);  
	USART_DMACmd(DEBUG_USART2, USART_DMAReq_Rx, ENABLE); 
	USART2_DMA_Rx_Config();
	USART2_DMA_Tx_Config();
	// ʹ�ܴ���
	USART_Cmd(DEBUG_USART2, ENABLE);	    
}


void USART2_DMA_Tx_Config(void)
{
		DMA_InitTypeDef DMA_InitStructure;
	
		// ����DMAʱ��
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
		// ����DMAԴ��ַ���������ݼĴ�����ַ*/
    DMA_InitStructure.DMA_PeripheralBaseAddr = USART2_DR_ADDRESS;
		// �ڴ��ַ(Ҫ����ı�����ָ��)
		DMA_InitStructure.DMA_MemoryBaseAddr = (u32)SendBuff2;
		// ���򣺴��ڴ浽����	
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
		// �����С	
		DMA_InitStructure.DMA_BufferSize = SENDBUFF2_SIZE;
		// �����ַ����	    
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
		// �ڴ��ַ����
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
		// �������ݵ�λ	
		DMA_InitStructure.DMA_PeripheralDataSize = 
	  DMA_PeripheralDataSize_Byte;
		// �ڴ����ݵ�λ
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;	 
		// DMAģʽ��һ�λ���ѭ��ģʽ
		DMA_InitStructure.DMA_Mode = DMA_Mode_Normal ;
		//DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	
		// ���ȼ�����	
		DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; 
		// ��ֹ�ڴ浽�ڴ�Ĵ���
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
		// ����DMAͨ��		   
		DMA_Init(USART2_TX_DMA_CHANNEL, &DMA_InitStructure);		
		// ʹ��DMA
		DMA_Cmd (USART2_TX_DMA_CHANNEL,ENABLE);
}

static void USART2_DMA_Rx_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;

	// ����DMAʱ��
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	// ����DMAԴ��ַ���������ݼĴ�����ַ*/
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)USART2_DR_ADDRESS;
	// �ڴ��ַ(Ҫ����ı�����ָ��)
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)Usart2_Rx_Buf;
	// ���򣺴��ڴ浽����	
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	// �����С	
	DMA_InitStructure.DMA_BufferSize = USART2_RX_BUFF_SIZE;
	// �����ַ����	    
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	// �ڴ��ַ����
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	// �������ݵ�λ	
	DMA_InitStructure.DMA_PeripheralDataSize = 
	DMA_PeripheralDataSize_Byte;
	// �ڴ����ݵ�λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;	 
	// DMAģʽ��һ�λ���ѭ��ģʽ
	//DMA_InitStructure.DMA_Mode = DMA_Mode_Normal ;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	
	// ���ȼ�����	
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh; 
	// ��ֹ�ڴ浽�ڴ�Ĵ���
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	// ����DMAͨ��		   
	DMA_Init(USART2_RX_DMA_CHANNEL, &DMA_InitStructure);		
	// ���DMA���б�־
	DMA_ClearFlag(DMA1_FLAG_TC5);
	DMA_ITConfig(USART2_RX_DMA_CHANNEL, DMA_IT_TE, ENABLE);
	// ʹ��DMA
	DMA_Cmd (USART2_RX_DMA_CHANNEL,ENABLE);
}

void DEBUG_USART2_IRQHandler(void)
{
	/* ʹ�ô���DMA */
	if(USART_GetITStatus(DEBUG_USART2,USART_IT_IDLE)!=RESET)
	{		
		/* �������� */
		USART2_Receive_DataPack();
		// ��������жϱ�־λ
		USART_ReceiveData( DEBUG_USART2 );
	}	

}

void Usart2_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	/* ����һ���ֽ����ݵ�USART */
	USART_SendData(pUSARTx,ch);
		
	/* �ȴ��������ݼĴ���Ϊ�� */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

void HMISendb(uint8_t k)		         //�ֽڷ��ͺ���
{		 
	uint8_t i;
	 for(i=0;i<3;i++)
	 {
	 if(k!=0)
	 	{
			USART_SendData(USART2,k);  //����һ���ֽ�
			while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET){};//�ȴ����ͽ���
		}
	 else 
	 return ;

	 } 
}

void HMISends(char *str)
{
	unsigned int k=0;
  do 
  {
      Usart2_SendByte( DEBUG_USART2, *(str + k) );
      k++;
  } while(*(str + k)!='\0');
	
  /* �ȴ�������� */
  while(USART_GetFlagStatus(DEBUG_USART2,USART_FLAG_TC)==RESET)
  {}
	HMISendb(0xff);
}

	
void USART2_Receive_DataPack(void)
{
	/* ���յ����ݳ��� */
	uint32_t buff_length;
	uint8_t *pbuff = Usart2_Rx_Buf;
	
	/* �ر�DMA ����ֹ���� */
	DMA_Cmd(USART2_RX_DMA_CHANNEL, DISABLE);  /* ��ʱ�ر�dma��������δ���� */ 
	
	/* ��ȡ���յ������ݳ��� ��λΪ�ֽ�*/
	buff_length = USART2_RX_BUFF_SIZE - DMA_GetCurrDataCounter(USART2_RX_DMA_CHANNEL);
	
	Usart2_data_process(pbuff);

	memset(Usart2_Rx_Buf,0,buff_length);
	
	/* ��DMA��־λ */
	DMA_ClearFlag( DMA1_FLAG_TC5 );          
	
	/* ���¸�ֵ����ֵ��������ڵ��������ܽ��յ�������֡��Ŀ */
	USART2_RX_DMA_CHANNEL->CNDTR = USART2_RX_BUFF_SIZE;    
  
	/* �˴�Ӧ���ڴ����������ٴ򿪣����� DataPack_Process() ��*/
	DMA_Cmd(USART2_RX_DMA_CHANNEL, ENABLE);      

}

void USART2_send(void)
{
	int i;
	for(i=0;i<SENDBUFF2_SIZE;i++)
  {
    SendBuff2[i]	 = '1';
  }
  DMA_Cmd(DMA1_Channel7, DISABLE);
                               
  DMA1_Channel7->CNDTR =  20;   //�����趨�������ݸ���  
                               
  DMA_Cmd(DMA1_Channel7, ENABLE);
		 	 
}


