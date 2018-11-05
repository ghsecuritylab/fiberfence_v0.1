#ifndef RT_CONFIG_H__
#define RT_CONFIG_H__

/* Automatically generated file; DO NOT EDIT. */
/* RT-Thread Configuration */

#define BOARD_STM32F429_APPOLO

/* RT-Thread Kernel */

#define RT_NAME_MAX 16
#define RT_ALIGN_SIZE 4
#define RT_THREAD_PRIORITY_32
#define RT_THREAD_PRIORITY_MAX 32
#define RT_TICK_PER_SECOND 1000
#define RT_DEBUG
#define RT_USING_OVERFLOW_CHECK
#define RT_DEBUG_INIT 1
#define RT_DEBUG_THREAD 0
#define RT_USING_HOOK
#define IDLE_THREAD_STACK_SIZE 1024

/* Inter-Thread communication */

#define RT_USING_SEMAPHORE
#define RT_USING_MUTEX
#define RT_USING_EVENT
#define RT_USING_MAILBOX
#define RT_USING_MESSAGEQUEUE

/* Memory Management */

#define RT_USING_MEMPOOL
#define RT_USING_MEMHEAP
#define RT_USING_SMALL_MEM
#define RT_USING_HEAP

/* Kernel Device Object */

#define RT_USING_DEVICE
#define RT_USING_CONSOLE
#define RT_CONSOLEBUF_SIZE 128
#define RT_CONSOLE_DEVICE_NAME "uart"
#define ARCH_ARM
#define ARCH_ARM_CORTEX_M
#define ARCH_ARM_CORTEX_M4

/* RT-Thread Components */

#define RT_USING_COMPONENTS_INIT

/* C++ features */


/* Command shell */

#define RT_USING_FINSH
#define FINSH_THREAD_NAME "tshell"
#define FINSH_USING_HISTORY
#define FINSH_HISTORY_LINES 5
#define FINSH_USING_SYMTAB
#define FINSH_USING_DESCRIPTION
#define FINSH_THREAD_PRIORITY 20
#define FINSH_THREAD_STACK_SIZE 2048
#define FINSH_CMD_SIZE 80
#define FINSH_USING_MSH
#define FINSH_USING_MSH_DEFAULT

/* Device virtual file system */


/* Device Drivers */

#define RT_USING_DEVICE_IPC
#define RT_USING_SERIAL
#define RT_USING_HWTIMER
#define RT_USING_I2C
#define RT_USING_I2C_BITOPS
#define RT_USING_RTC
#define RT_USING_SPI
#define RT_USING_W25QXX

/* Using USB */


/* POSIX layer and C standard library */


/* Network stack */

/* light weight TCP/IP stack */


/* Modbus master and slave stack */


/* VBUS(Virtual Software BUS) */


/* Utilities */


/* ARM CMSIS */


/* RT-Thread online packages */

/* system packages */

/* RT-Thread GUI Engine */


/* IoT - internet of things */


/* Wi-Fi */

/* Marvell WiFi */


/* Wiced WiFi */


/* security packages */


/* language packages */


/* multimedia packages */


/* tools packages */


/* miscellaneous packages */


/* example package: hello */

#define RT_USING_UART1
#define RT_USING_UART2
#define RT_USING_UART3
#define RT_USING_UART6
#define RT_USING_SPI2
#define RT_USING_SPI3
#define RT_USING_SPI5
#define RT_USING_SPI6
#define RT_RTC_NAME "rtc"
#define RT_USING_I2C1
#define RT_USING_I2C2
#define RT_USING_I2C3

#endif
