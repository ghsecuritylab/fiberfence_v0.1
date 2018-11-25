

1、硬件引脚分配：

/********** SPI PIN Configuration ***************
		SPI2_MOSI ------> PI3	//DAC1
		SPI2_MISO ------> PI2
		SPI2_CLK  ------> PI1
		SPI2_SYNC ------> PI0

		SPI3_MOSI ------> PB5	//DAC2
		SPI3_MISO ------> PB4
		SPI3_CLK  ------> PB3
		SPI3_SYNC ------> PE0

		SPI5_MOSI ------> PF9	//flash
		SPI5_MISO ------> PF8
		SPI5_CLK  ------> PF7
		SPI5_SYNC ------> PF10

		SPI6_MOSI ------> PG14	//ADC
		SPI6_MISO ------> PG12
		SPI6_CLK  ------> PG13
		SPI6_SYNC ------> PG15
**************************************************/

/********** IIC PIN Configuration ***************
		IIC1_CLK ------> PB6	//EEPROM
		IIC1_SDA ------> PB7

		IIC3_CLK ------> PH7	//保留
		IIC3_SDA ------> PH8
**************************************************/

/********** UART PIN Configuration ***************
		UART1_RX ------> PA10
		UART1_TX ------> PA9	//用于485
		485_TX_EN------> PA8

		UART3_RX ------> PB6	//用于GSM
		UART3_TX ------> PB7
		
		UART6_RX ------> PC6	//用于debug
		UART6_TX ------> PC7
**************************************************/

/********** MII PIN Configuration ***************
		PA0     ------> ETH_CRS
        PA1     ------> ETH_RX_CLK
        PA2     ------> ETH_MDIO
        PA3     ------> ETH_COL
		PA7     ------> ETH_RXDV
		
		PB0      ------> ETH_RXD2
        PB1      ------> ETH_RXD3
		PB8      ------> ETH_TXD3
        PB10     ------> ETH_RXER
		PB11     ------> ETH_TXEN
        PB12     ------> ETH_TXD0
		PB13     ------> ETH_TXD1
				
		PC0     ------> PHY_REST
		PC1     ------> ETH_MDC
        PC2     ------> ETH_TXD2
        PC3     ------> ETH_TX_CLK
        PC4     ------> ETH_RXD0
        PC5     ------> ETH_RXD1	
**************************************************/

/********** LCD1602 PIN Configuration ***************
		1602D0 ------> PE5
		1602D1 ------> PE4
		1602D2 ------> PE3
		1602D3 ------> PI7
		1602D4 ------> PI6
		1602D5 ------> PI5
		1602D6 ------> PI4
		1602D7 ------> PE1
		1602RW ------> PI8
		1602RS ------> PC13
		1602EN ------> PE6
**************************************************/

/********** JDQ PIN Configuration ***************
		JDQ1 ------> PI9	//防区A报警输出
		JDQ2 ------> PI10	//防区B报警输出
**************************************************/

/********** LED PIN Configuration ***************
		LED7  ------> PG3	//防区B报警led
		LED8  ------> PG4	//防区B光路led
		LED12 ------> PG5	//防区A报警led
		LED13 ------> PG6	//防区A报警led
		LED14 ------> PG7	//网络运行led
		LED9  ------> PG8	//系统运行led
		LED10 ------> PG10	//CPU运行led
		LED11 ------> PG11	//CPU报警led
**************************************************/

/********** KEYS PIN Configuration ***************
		KEY1  ------> PD10	//BACK
		KEY2  ------> PD11	//ENTER
		KEY3  ------> PD12	//SUB
		KEY4  ------> PD13	//PLUS
		KEY5  ------> PD14	//DOWN
		KEY6  ------> PD15	//UP
**************************************************/

