#ifndef __MYSPI_H
#define __MYSPI_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F429������
//SPI��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2016/16/16
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

	extern SPI_HandleTypeDef SPI2_Handler;  //SPI���

	void SPI2_Init(void);
	void SPI2_SetSpeed(u8 SPI_BaudRatePrescaler);
	u16 SPI2_ReadWriteWord(u16 TxData);

	extern SPI_HandleTypeDef SPI5_Handler;  //SPI���

	void SPI5_Init(void);
	void SPI5_SetSpeed(u8 SPI_BaudRatePrescaler);
	u8 SPI5_ReadWriteByte(u8 TxData);

	extern SPI_HandleTypeDef SPI6_Handler;  //SPI���

	void SPI6_Init(void);
	void SPI6_SetSpeed(u8 SPI_BaudRatePrescaler);
	u16 SPI6_ReadWriteWord(u16 TxData);

#endif
