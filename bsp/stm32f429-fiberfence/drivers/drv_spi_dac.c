#include <board.h>
#include "drv_spi.h"

#include <rthw.h>
#include <finsh.h>

#define SPI_BUS_NAME2              "spi2"
#define SPI_DAC_DEVICE_NAME2       "spi20"

#define SPI_BUS_NAME3              "spi3"
#define SPI_DAC_DEVICE_NAME3       "spi30"

static struct rt_spi_device *dac_spi_device2, *dac_spi_device3;

/**********GPIO Configuration***************

		SPI2_MOSI ------> PI3
		SPI2_MISO ------> PI2
		SPI2_CLK  ------> PI1
		SPI2_SYNC ------> PI0

		SPI3_MOSI ------> PB5
		SPI3_MISO ------> PB4
		SPI3_CLK  ------> PB3
		SPI3_SYNC ------> PE0

		SPI5_MOSI ------> PF9
		SPI5_MISO ------> PF8
		SPI5_CLK  ------> PF7
		SPI5_SYNC ------> PF10

		SPI6_MOSI ------> PG14
		SPI6_MISO ------> PG12
		SPI6_CLK  ------> PG13
		SPI6_SYNC ------> PG15

*******************************************/

static int rt_hw_spi2_init(void)
{
    /* register spi bus */
    {
        GPIO_InitTypeDef GPIO_InitStructure;
				rt_err_t result;

				__HAL_RCC_GPIOI_CLK_ENABLE();

				GPIO_InitStructure.Pin = GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3;
				GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
				GPIO_InitStructure.Pull = GPIO_NOPULL;
				GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
				GPIO_InitStructure.Alternate = GPIO_AF5_SPI2;
				HAL_GPIO_Init(GPIOI, &GPIO_InitStructure);

				result = stm32_spi_bus_register(SPI2, SPI_BUS_NAME2);
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
        __HAL_RCC_GPIOI_CLK_ENABLE();
        
        spi_cs.GPIOx = GPIOI;
        spi_cs.GPIO_Pin = GPIO_PIN_0;
        
        GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStructure.Pull  = GPIO_PULLUP;
        GPIO_InitStructure.Speed = GPIO_SPEED_FAST;
        GPIO_InitStructure.Pin = spi_cs.GPIO_Pin;
        HAL_GPIO_Init(spi_cs.GPIOx, &GPIO_InitStructure);
        HAL_GPIO_WritePin(spi_cs.GPIOx, spi_cs.GPIO_Pin, GPIO_PIN_SET);

        result = rt_spi_bus_attach_device(&spi_device, SPI_DAC_DEVICE_NAME2, SPI_BUS_NAME2, (void*)&spi_cs);
				if (result != RT_EOK)
				{
					return result;
				}
    }

		return RT_EOK;
}
INIT_DEVICE_EXPORT(rt_hw_spi2_init);

static int rt_hw_spi3_init(void)
{
    /* register spi bus */
    {
        GPIO_InitTypeDef GPIO_InitStructure;
				rt_err_t result;

				__HAL_RCC_GPIOB_CLK_ENABLE();

				GPIO_InitStructure.Pin = GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5;
				GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
				GPIO_InitStructure.Pull = GPIO_NOPULL;
				GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
				GPIO_InitStructure.Alternate = GPIO_AF6_SPI3;
				HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

				result = stm32_spi_bus_register(SPI3, SPI_BUS_NAME3);
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
        __HAL_RCC_GPIOE_CLK_ENABLE();
        
        spi_cs.GPIOx = GPIOE;
        spi_cs.GPIO_Pin = GPIO_PIN_0;
        
        GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStructure.Pull  = GPIO_PULLUP;
        GPIO_InitStructure.Speed = GPIO_SPEED_FAST;
        GPIO_InitStructure.Pin = spi_cs.GPIO_Pin;
        HAL_GPIO_Init(spi_cs.GPIOx, &GPIO_InitStructure);
        HAL_GPIO_WritePin(spi_cs.GPIOx, spi_cs.GPIO_Pin, GPIO_PIN_SET);

        result = rt_spi_bus_attach_device(&spi_device, SPI_DAC_DEVICE_NAME3, SPI_BUS_NAME3, (void*)&spi_cs);
				if (result != RT_EOK)
				{
					return result;
				}
    }

		return RT_EOK;
}
INIT_DEVICE_EXPORT(rt_hw_spi3_init);

int dac2_init()
{
	dac_spi_device2 = (struct rt_spi_device*)rt_device_find(SPI_DAC_DEVICE_NAME2);
	if(dac_spi_device2==NULL)
	{
		rt_kprintf("spi_device can not find!\n");
		return -1;
	}
	
	rt_device_open(&dac_spi_device2->parent, RT_DEVICE_FLAG_RDWR);
	//rt_device_close(&adc_spi_device->parent);
	
	/* config spi */
	{
			struct rt_spi_configuration cfg;
			cfg.data_width = 16;
			cfg.mode = RT_SPI_MODE_2 | RT_SPI_MSB | RT_SPI_MASTER; /* SPI Compatible: Mode 0 and Mode 3 */
			cfg.max_hz = 50 * 1000 * 1000; /* 50M */
			rt_spi_configure(dac_spi_device2, &cfg);
	}
	
	
	return 0;
}
INIT_COMPONENT_EXPORT(dac2_init)

int dac3_init()
{
	dac_spi_device3 = (struct rt_spi_device*)rt_device_find(SPI_DAC_DEVICE_NAME3);
	if(dac_spi_device3==NULL)
	{
		rt_kprintf("spi_device can not find!\n");
		return -1;
	}
	
	rt_device_open(&dac_spi_device3->parent, RT_DEVICE_FLAG_RDWR);
	//rt_device_close(&adc_spi_device->parent);
	
	/* config spi */
	{
			struct rt_spi_configuration cfg;
			cfg.data_width = 16;
			cfg.mode = RT_SPI_MODE_2 | RT_SPI_MSB | RT_SPI_MASTER; /* SPI Compatible: Mode 0 and Mode 3 */
			cfg.max_hz = 50 * 1000 * 1000; /* 50M */
			rt_spi_configure(dac_spi_device3, &cfg);
	}
	
	
	return 0;
}
INIT_COMPONENT_EXPORT(dac3_init)

int set_dac(rt_uint16_t value, rt_uint16_t chip_id)
{
	struct rt_spi_device *device;
	if(chip_id==2)
		device = (struct rt_spi_device *) rt_device_find(SPI_DAC_DEVICE_NAME2);
	else if(chip_id==3)
		device = (struct rt_spi_device *) rt_device_find(SPI_DAC_DEVICE_NAME3);
	else
		return -1;
	
	rt_spi_send(device, &value, 1);
	
	return 0;
}

#ifdef RT_USING_FINSH
#include <finsh.h>
FINSH_FUNCTION_EXPORT_ALIAS(set_dac, set_dac, set value of dac_7513);
#endif

