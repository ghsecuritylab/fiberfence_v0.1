/*
 * File      : drv_iic.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2017 RT-Thread Develop Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2017-06-05     tanek        first implementation.
 */
 
#include "drv_iic.h"

#include <board.h>
#include <finsh.h>
#include <rtdevice.h>
#include <rthw.h>

#define DEBUG

#ifdef DEBUG
#define DEBUG_PRINTF(...)   rt_kprintf(__VA_ARGS__)
#else
#define DEBUG_PRINTF(...)   
#endif

static void stm32f4_set_sda(void *data, rt_int32_t state);
static void stm32f4_set_scl(void *data, rt_int32_t state);
static rt_int32_t  stm32f4_get_scl(void *data);
static rt_int32_t  stm32f4_get_sda(void *data);
static void stm32f4_udelay(rt_uint32_t us);

typedef struct
{
	GPIO_TypeDef *port;
	uint32_t pin_sda;
	uint32_t pin_scl;
}I2C_GPIO_Def;

#ifdef RT_USING_I2C1
static I2C_GPIO_Def i2c1_GPIO;
static const struct rt_i2c_bit_ops bit_ops1 = {
	&i2c1_GPIO,
	stm32f4_set_sda,
	stm32f4_set_scl,
	stm32f4_get_sda,
	stm32f4_get_scl,
	stm32f4_udelay,
	5,
	100
};
#endif

#ifdef RT_USING_I2C3
static I2C_GPIO_Def i2c3_GPIO;
static const struct rt_i2c_bit_ops bit_ops3 = {
	&i2c3_GPIO,
	stm32f4_set_sda,
	stm32f4_set_scl,
	stm32f4_get_sda,
	stm32f4_get_scl,
	stm32f4_udelay,
	5,
	100
};
#endif


/**********GPIO Configuration***************
		IIC1_CLK ------> PB6
		IIC1_SDA ------> PB7

		IIC3_CLK ------> PH7
		IIC3_SDA ------> PH8

*******************************************/

static void stm32f4_i2c_gpio_init()
{
	GPIO_InitTypeDef GPIO_Initure;
#ifdef RT_USING_I2C1

	__HAL_RCC_GPIOB_CLK_ENABLE();
    
	GPIO_Initure.Pin = GPIO_PIN_6 | GPIO_PIN_7;
	GPIO_Initure.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_Initure.Pull = GPIO_PULLUP;
	GPIO_Initure.Speed = GPIO_SPEED_FAST;
	HAL_GPIO_Init(GPIOB, &GPIO_Initure);
    
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);
	
	i2c1_GPIO.port = GPIOB;
	i2c1_GPIO.pin_sda = GPIO_PIN_7;
	i2c1_GPIO.pin_scl = GPIO_PIN_6;
#endif
	
#ifdef RT_USING_I2C3
	__HAL_RCC_GPIOH_CLK_ENABLE();
    
	GPIO_Initure.Pin = GPIO_PIN_7 | GPIO_PIN_8;
//	GPIO_Initure.Mode = GPIO_MODE_OUTPUT_OD;
//	GPIO_Initure.Pull = GPIO_PULLUP;
//	GPIO_Initure.Speed = GPIO_SPEED_FAST;
	HAL_GPIO_Init(GPIOH, &GPIO_Initure);
    
	HAL_GPIO_WritePin(GPIOH, GPIO_PIN_8, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOH, GPIO_PIN_7, GPIO_PIN_SET);
	
	i2c3_GPIO.port = GPIOH;
	i2c3_GPIO.pin_sda = GPIO_PIN_8;
	i2c3_GPIO.pin_scl = GPIO_PIN_7;
#endif
}

static void stm32f4_set_sda(void *data, rt_int32_t state)
{
	I2C_GPIO_Def *i2c_GPIO = (I2C_GPIO_Def *)data;
	HAL_GPIO_WritePin(i2c_GPIO->port, i2c_GPIO->pin_sda, (GPIO_PinState)state);
}

static void stm32f4_set_scl(void *data, rt_int32_t state)
{
	I2C_GPIO_Def *i2c_GPIO = (I2C_GPIO_Def *)data;
	HAL_GPIO_WritePin(i2c_GPIO->port, i2c_GPIO->pin_scl, (GPIO_PinState)state);
}

static rt_int32_t  stm32f4_get_sda(void *data)
{
	I2C_GPIO_Def *i2c_GPIO = (I2C_GPIO_Def *)data;
	return (rt_int32_t)HAL_GPIO_ReadPin(i2c_GPIO->port, i2c_GPIO->pin_sda);
}

static rt_int32_t  stm32f4_get_scl(void *data)
{
	I2C_GPIO_Def *i2c_GPIO = (I2C_GPIO_Def *)data;
	return (rt_int32_t)HAL_GPIO_ReadPin(i2c_GPIO->port, i2c_GPIO->pin_scl);
}

static void stm32f4_udelay(rt_uint32_t us)
{
	rt_int32_t i;
	for (; us > 0; us--)
	{
		i = 50;
		while (i > 0)
		{
			i--;
		}
	}
}


int stm32f4_i2c_init(void)
{
	stm32f4_i2c_gpio_init();
	
#ifdef RT_USING_I2C1
	struct rt_i2c_bus_device *bus1;
	
	bus1 = rt_malloc(sizeof(struct rt_i2c_bus_device));
	if (bus1 == RT_NULL)
	{
		rt_kprintf("rt_malloc failed\n");
		return -RT_ENOMEM;
	}

	rt_memset((void *)bus1, 0, sizeof(struct rt_i2c_bus_device));
	bus1->priv = (void *)&bit_ops1;
	rt_i2c_bit_add_bus(bus1, "i2c1");
#endif
	
#ifdef RT_USING_I2C3
	struct rt_i2c_bus_device *bus3;
	
	bus3 = rt_malloc(sizeof(struct rt_i2c_bus_device));
	if (bus3 == RT_NULL)
	{
		rt_kprintf("rt_malloc failed\n");
		return -RT_ENOMEM;
	}

	rt_memset((void *)bus3, 0, sizeof(struct rt_i2c_bus_device));
	bus3->priv = (void *)&bit_ops3;
	rt_i2c_bit_add_bus(bus3, "i2c3");
#endif

	return RT_EOK;
}
INIT_DEVICE_EXPORT(stm32f4_i2c_init);
