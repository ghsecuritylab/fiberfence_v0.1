#include "uart6_dma.h"
#include <rtthread.h>

extern rt_thread_t alarmid_A;
extern rt_thread_t alarmid_B;

DMA_HandleTypeDef  UART6TxDMA_Handler;      //DMA���
UART_HandleTypeDef UART6_Handler;           //UART���

//��ʼ��IO ����6
//bound:������
void uart6_dma_init(uint32_t bound)
{	
	//UART ��ʼ������
	UART6_Handler.Instance=USART6;					            //USART6
	UART6_Handler.Init.BaudRate=bound;				          //������
	UART6_Handler.Init.WordLength=UART_WORDLENGTH_8B;   //�ֳ�Ϊ8λ���ݸ�ʽ
	UART6_Handler.Init.StopBits=UART_STOPBITS_1;	      //һ��ֹͣλ
	UART6_Handler.Init.Parity=UART_PARITY_NONE;		      //����żУ��λ
	UART6_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   //��Ӳ������
	UART6_Handler.Init.Mode=UART_MODE_TX_RX;		        //�շ�ģʽ
	HAL_UART_Init(&UART6_Handler);					            //HAL_UART_Init()��ʹ��UART6
  
  __HAL_RCC_DMA2_CLK_ENABLE();//DMA2ʱ��ʹ��	
    
	__HAL_LINKDMA(&UART6_Handler,hdmatx,UART6TxDMA_Handler);            //��DMA��USART1��ϵ����(����DMA)
	
	//Tx DMA����
	UART6TxDMA_Handler.Instance=DMA2_Stream7;                           //������ѡ��
	UART6TxDMA_Handler.Init.Channel=DMA_CHANNEL_5;                      //ͨ��ѡ��
	UART6TxDMA_Handler.Init.Direction=DMA_MEMORY_TO_PERIPH;             //�洢��������
	UART6TxDMA_Handler.Init.PeriphInc=DMA_PINC_DISABLE;                 //���������ģʽ
	UART6TxDMA_Handler.Init.MemInc=DMA_MINC_ENABLE;                     //�洢������ģʽ
	UART6TxDMA_Handler.Init.PeriphDataAlignment=DMA_PDATAALIGN_BYTE;    //�������ݳ���:8λ
	UART6TxDMA_Handler.Init.MemDataAlignment=DMA_MDATAALIGN_BYTE;       //�洢�����ݳ���:8λ
	UART6TxDMA_Handler.Init.Mode=DMA_NORMAL;                            //������ͨģʽ
	UART6TxDMA_Handler.Init.Priority=DMA_PRIORITY_MEDIUM;               //�е����ȼ�
	UART6TxDMA_Handler.Init.FIFOMode=DMA_FIFOMODE_DISABLE;              
	UART6TxDMA_Handler.Init.FIFOThreshold=DMA_FIFO_THRESHOLD_FULL;      
	UART6TxDMA_Handler.Init.MemBurst=DMA_MBURST_SINGLE;                 //�洢��ͻ�����δ���
	UART6TxDMA_Handler.Init.PeriphBurst=DMA_PBURST_SINGLE;              //����ͻ�����δ���
	
	HAL_DMA_DeInit(&UART6TxDMA_Handler);   
	HAL_DMA_Init(&UART6TxDMA_Handler);
	
	/* DMA interrupt init */
	/* DMA2_Stream7_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA2_Stream7_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA2_Stream7_IRQn);
	
}

////��Ϊ��usart.c�г�ʼ����UART6���������ã�����ע�͵�
////UART�ײ��ʼ����ʱ��ʹ�ܣ��������ã��ж�����
////�˺����ᱻHAL_UART_Init()����
////huart:���ھ��

//void HAL_UART_MspInit(UART_HandleTypeDef *huart)
//{
//    //GPIO�˿�����
//	GPIO_InitTypeDef GPIO_Initure;
//	
//	if(huart->Instance==USART6)             //����Ǵ���6�����д���6 MSP��ʼ��
//	{
//		__HAL_RCC_GPIOC_CLK_ENABLE();		     	//ʹ��GPIOAʱ��
//		__HAL_RCC_USART6_CLK_ENABLE();		  	//ʹ��USART6ʱ��
//	
//		GPIO_Initure.Pin=GPIO_PIN_6;		    	//PC6
//		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//�����������
//		GPIO_Initure.Pull=GPIO_PULLUP;			  //����
//		GPIO_Initure.Speed=GPIO_SPEED_FAST;		//����
//		GPIO_Initure.Alternate=GPIO_AF8_USART6;	  //����ΪUSART6
//		HAL_GPIO_Init(GPIOC,&GPIO_Initure);	     	//��ʼ��PC6

//		GPIO_Initure.Pin=GPIO_PIN_7;			        //PC7
//		HAL_GPIO_Init(GPIOC,&GPIO_Initure);	    	//��ʼ��PC7
//		__HAL_UART_DISABLE_IT(huart,UART_IT_TC);
//#if EN_USART1_RX
//		__HAL_UART_ENABLE_IT(huart,UART_IT_RXNE);	//���������ж�
//		HAL_NVIC_EnableIRQ(USART6_IRQn);			  	//ʹ��USART6�ж�ͨ��
//		HAL_NVIC_SetPriority(USART6_IRQn,3,3);		//��ռ���ȼ�3�������ȼ�3
//#endif	
//	}

//}

void DMA2_Stream7_IRQHandler(void)
{
	HAL_DMA_IRQHandler(&UART6TxDMA_Handler);
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	rt_thread_resume(alarmid_A);
}
