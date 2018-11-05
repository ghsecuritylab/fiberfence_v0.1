#include <board.h>
#include "drv_spi.h"

#include <rthw.h>
#include <finsh.h>

//ADC cmd
/*--------------------------------------
  |  15 | 14 |  13 |  12 | 11 | 10 | 9 | 8 |  7 |  6  |  5  |   4  | 3 | 2 | 1 | 0 |
	|WRITE|SEQ1|DONTC|DONTC|ADD1|ADD0|PM1|PM0|SEQ0|DONTC|RANGE|CODING| 0 | 0 | 0 | 0 |
	
	RANGE:  0 2*ref
				  1 ref
	CODING: 0 补码
				  1 标准二进制
*---------------------------------------*/
#define CHAN1_CONVERSION    0x8310      //0x8330-ref=2.5v   0x8310-2*ref=5v
#define CHAN2_CONVERSION    0x8730      //0x8730-ref=2.5v   0x8710-2*ref=5v
#define CHAN3_CONVERSION    0x8B30      //0x8B30-ref=2.5v   0x8B10-2*ref=5v
#define CHAN4_CONVERSION    0x8F10      //0x8F30-ref=2.5v   0x8F10-2*ref=5v


#define SPI_BUS_NAME              "spi6"
#define SPI_ADC_DEVICE_NAME       "spi60"
#define SPI_xxx_DEVICE_NAME       "spi61"
#define SPI_ADC_CHIP              "ADC7924"

static struct rt_spi_device *adc_spi_device, *xxx_spi_device;

static int rt_hw_spi6_init(void)
{
    /* register spi bus */
    {
        GPIO_InitTypeDef GPIO_InitStructure;
				rt_err_t result;

				__HAL_RCC_GPIOG_CLK_ENABLE();

				GPIO_InitStructure.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14;
				GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
				GPIO_InitStructure.Pull = GPIO_NOPULL;
				GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
				GPIO_InitStructure.Alternate = GPIO_AF5_SPI6;
				HAL_GPIO_Init(GPIOG, &GPIO_InitStructure);

				result = stm32_spi_bus_register(SPI6, SPI_BUS_NAME);
				if (result != RT_EOK)
				{
					return result;
				}
    }

    /* attach cs */
    {
        static struct rt_spi_device spi_device;
        static struct stm32_spi_cs  spi_cs;
				rt_err_t result;

        GPIO_InitTypeDef GPIO_InitStructure;
        __HAL_RCC_GPIOG_CLK_ENABLE();
        
        spi_cs.GPIOx = GPIOG;
        spi_cs.GPIO_Pin = GPIO_PIN_15;
        
        GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStructure.Pull  = GPIO_PULLUP;
        GPIO_InitStructure.Speed = GPIO_SPEED_FAST;
        GPIO_InitStructure.Pin = spi_cs.GPIO_Pin;
        HAL_GPIO_Init(spi_cs.GPIOx, &GPIO_InitStructure);
        HAL_GPIO_WritePin(spi_cs.GPIOx, spi_cs.GPIO_Pin, GPIO_PIN_SET);

        result = rt_spi_bus_attach_device(&spi_device, SPI_ADC_DEVICE_NAME, SPI_BUS_NAME, (void*)&spi_cs);
				if (result != RT_EOK)
				{
					return result;
				}
    }
		
		/* attach cs */
    {
        static struct rt_spi_device spi_device1;
        static struct stm32_spi_cs  spi_cs1;
				rt_err_t result;

        GPIO_InitTypeDef GPIO_InitStructure;
        __HAL_RCC_GPIOG_CLK_ENABLE();
        
        spi_cs1.GPIOx = GPIOG;
        spi_cs1.GPIO_Pin = GPIO_PIN_9;
        
        GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStructure.Pull  = GPIO_PULLUP;
        GPIO_InitStructure.Speed = GPIO_SPEED_FAST;
        GPIO_InitStructure.Pin = spi_cs1.GPIO_Pin;
        HAL_GPIO_Init(spi_cs1.GPIOx, &GPIO_InitStructure);
        HAL_GPIO_WritePin(spi_cs1.GPIOx, spi_cs1.GPIO_Pin, GPIO_PIN_SET);

        result = rt_spi_bus_attach_device(&spi_device1, SPI_xxx_DEVICE_NAME, SPI_BUS_NAME, (void*)&spi_cs1);
				if (result != RT_EOK)
				{
					return result;
				}
    }

		return RT_EOK;
}
INIT_DEVICE_EXPORT(rt_hw_spi6_init);

int adc7924_init()
{
	adc_spi_device = (struct rt_spi_device*)rt_device_find(SPI_ADC_DEVICE_NAME);
	if(adc_spi_device==NULL)
	{
		rt_kprintf("spi_device can not find!\n");
		return -1;
	}
	
	//rt_device_open(&adc_spi_device->parent, RT_DEVICE_FLAG_RDWR);
	//rt_device_close(&adc_spi_device->parent);
	
	/* config spi */
	{
			struct rt_spi_configuration cfg;
			cfg.data_width = 16;
			cfg.mode = RT_SPI_MODE_2 | RT_SPI_MSB | RT_SPI_MASTER; /* SPI Compatible: Mode 0 and Mode 3 */
			cfg.max_hz = 50 * 1000 * 1000; /* 50M */
			rt_spi_configure(adc_spi_device, &cfg);
	}
	
	
	return 0;
}
INIT_COMPONENT_EXPORT(adc7924_init)

//int xxx_init()
//{
//	xxx_spi_device = (struct rt_spi_device*)rt_device_find(SPI_xxx_DEVICE_NAME);
//	if(xxx_spi_device==NULL)
//	{
//		rt_kprintf("spi_device_xxx can not find!\n");
//		return -1;
//	}
//	
//	/* config spi */
//	{
//			struct rt_spi_configuration cfg;
//			cfg.data_width = 16;
//			cfg.mode = RT_SPI_MODE_2 | RT_SPI_MSB | RT_SPI_MASTER; /* SPI Compatible: Mode 0 and Mode 3 */
//			cfg.max_hz = 100 * 1000 * 1000; /* 50M */
//			rt_spi_configure(xxx_spi_device, &cfg);
//	}
//	
//	rt_spi_take_bus(xxx_spi_device);
//	rt_spi_release_bus(xxx_spi_device);
//	
//	
//	return 0;
//}
//INIT_COMPONENT_EXPORT(xxx_init)

rt_uint16_t adc_ReadOneSample(rt_uint16_t cmd)
{
	rt_uint16_t recv, send;
	
	send = cmd;
	//rt_spi_take_bus(adc_spi_device);      //占用总线
	rt_spi_transfer(adc_spi_device, &send, &recv, 1);
	//rt_spi_release_bus(adc_spi_device);   //释放总线
	return recv&0xfff;
}
#ifdef RT_USING_FINSH
#include <finsh.h>
FINSH_FUNCTION_EXPORT_ALIAS(adc_ReadOneSample, radc, read a word from adc);
#endif
//RTM_EXPORT(adc_ReadOneSample)
