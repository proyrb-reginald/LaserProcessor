#ifndef SDRAM_H
#define SDRAM_H

/* 导入外部库 */
#define SDRAM_HAL_INC <stm32h7xx_hal.h>
#include SDRAM_HAL_INC
#define SDRAM_BSP_INC <board.h>
#include SDRAM_BSP_INC
#define SDRAM_RTOS_INC <rtthread.h>
#include SDRAM_RTOS_INC

/* 定义数据类型接口 */
#define SDRAM_UINT8_TYPE  uint8_t
#define SDRAM_UINT16_TYPE uint16_t
#define SDRAM_UINT32_TYPE uint32_t

/* 配置串口日志 */
#define SDRAM_LOG 1
#if SDRAM_LOG == 1
/* 串口日志接口 */
#define SDRAM_LOG_INTERFACE rt_lprintf
#else
#define SDRAM_LOG_INTERFACE(...)
#endif

/* 提供延时函数 */
#define SDRAM_DELAY_INTERFACE rt_thread_mdelay

/* SDRAM信息 */
#define SDRAM_ADDR ((SDRAM_UINT8_TYPE *)(0xC0000000))
#define SDRAM_SIZE ((SDRAM_UINT32_TYPE)(32 * 1024 * 1024))


void sdram_init(void);

void sdram_test(void);

/*  */

#endif