#include "uart6_dma.h"
#include <rtthread.h>

extern rt_thread_t alarmid_A;
extern rt_thread_t alarmid_B;

DMA_HandleTypeDef  UART6TxDMA_Handler;      //DMA句柄
UART_HandleTypeDef UART6_Handler;           //UART句柄

//初始化IO 串口6
//bound:波特率
void uart6_dma_init(uint32_t bound)
{	
	//UART 初始化设置
	UART6_Handler.Instance=USART6;					            //USART6
	UART6_Handler.Init.BaudRate=bound;				          //波特率
	UART6_Handler.Init.WordLength=UART_WORDLENGTH_8B;   //字长为8位数据格式
	UART6_Handler.Init.StopBits=UART_STOPBITS_1;	      //一个停止位
	UART6_Handler.Init.Parity=UART_PARITY_NONE;		      //无奇偶校验位
	UART6_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   //无硬件流控
	UART6_Handler.Init.Mode=UART_MODE_TX_RX;		        //收发模式
	HAL_UART_Init(&UART6_Handler);					            //HAL_UART_Init()会使能UART6
  
  __HAL_RCC_DMA2_CLK_ENABLE();//DMA2时钟使能	
    
	__HAL_LINKDMA(&UART6_Handler,hdmatx,UART6TxDMA_Handler);            //将DMA与USART1联系起来(发送DMA)
	
	//Tx DMA配置
	UART6TxDMA_Handler.Instance=DMA2_Stream7;                           //数据流选择
	UART6TxDMA_Handler.Init.Channel=DMA_CHANNEL_5;                      //通道选择
	UART6TxDMA_Handler.Init.Direction=DMA_MEMORY_TO_PERIPH;             //存储器到外设
	UART6TxDMA_Handler.Init.PeriphInc=DMA_PINC_DISABLE;                 //外设非增量模式
	UART6TxDMA_Handler.Init.MemInc=DMA_MINC_ENABLE;                     //存储器增量模式
	UART6TxDMA_Handler.Init.PeriphDataAlignment=DMA_PDATAALIGN_BYTE;    //外设数据长度:8位
	UART6TxDMA_Handler.Init.MemDataAlignment=DMA_MDATAALIGN_BYTE;       //存储器数据长度:8位
	UART6TxDMA_Handler.Init.Mode=DMA_NORMAL;                            //外设普通模式
	UART6TxDMA_Handler.Init.Priority=DMA_PRIORITY_MEDIUM;               //中等优先级
	UART6TxDMA_Handler.Init.FIFOMode=DMA_FIFOMODE_DISABLE;              
	UART6TxDMA_Handler.Init.FIFOThreshold=DMA_FIFO_THRESHOLD_FULL;      
	UART6TxDMA_Handler.Init.MemBurst=DMA_MBURST_SINGLE;                 //存储器突发单次传输
	UART6TxDMA_Handler.Init.PeriphBurst=DMA_PBURST_SINGLE;              //外设突发单次传输
	
	HAL_DMA_DeInit(&UART6TxDMA_Handler);   
	HAL_DMA_Init(&UART6TxDMA_Handler);
	
	/* DMA interrupt init */
	/* DMA2_Stream7_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA2_Stream7_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA2_Stream7_IRQn);
	
}

////因为在usart.c中初始化了UART6的引脚配置，这里注释掉
////UART底层初始化，时钟使能，引脚配置，中断配置
////此函数会被HAL_UART_Init()调用
////huart:串口句柄

//void HAL_UART_MspInit(UART_HandleTypeDef *huart)
//{
//    //GPIO端口设置
//	GPIO_InitTypeDef GPIO_Initure;
//	
//	if(huart->Instance==USART6)             //如果是串口6，进行串口6 MSP初始化
//	{
//		__HAL_RCC_GPIOC_CLK_ENABLE();		     	//使能GPIOA时钟
//		__HAL_RCC_USART6_CLK_ENABLE();		  	//使能USART6时钟
//	
//		GPIO_Initure.Pin=GPIO_PIN_6;		    	//PC6
//		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//复用推挽输出
//		GPIO_Initure.Pull=GPIO_PULLUP;			  //上拉
//		GPIO_Initure.Speed=GPIO_SPEED_FAST;		//高速
//		GPIO_Initure.Alternate=GPIO_AF8_USART6;	  //复用为USART6
//		HAL_GPIO_Init(GPIOC,&GPIO_Initure);	     	//初始化PC6

//		GPIO_Initure.Pin=GPIO_PIN_7;			        //PC7
//		HAL_GPIO_Init(GPIOC,&GPIO_Initure);	    	//初始化PC7
//		__HAL_UART_DISABLE_IT(huart,UART_IT_TC);
//#if EN_USART1_RX
//		__HAL_UART_ENABLE_IT(huart,UART_IT_RXNE);	//开启接收中断
//		HAL_NVIC_EnableIRQ(USART6_IRQn);			  	//使能USART6中断通道
//		HAL_NVIC_SetPriority(USART6_IRQn,3,3);		//抢占优先级3，子优先级3
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
