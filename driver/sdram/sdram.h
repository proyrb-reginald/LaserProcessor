#ifndef SDRAM_H
#define SDRAM_H

/* 导入外部库 */
#define ADS1118_HAL_INC <stm32h7xx_hal.h>
#include ADS1118_HAL_INC
#define ADS1118_BSP_INC <board.h>
#include ADS1118_BSP_INC
#define ADS1118_RTOS_INC <rtthread.h>
#include ADS1118_RTOS_INC

#define SDRAM_ADDR      ((uint32_t)0xC0000000)
#define SDRAM_SIZE      (32 * 1024 * 1024)
#define SDRAM_TEST_SIZE 16

/* SDRAM配置参数 */
#define SDRAM_MODEREG_BURST_LENGTH_1             ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_LENGTH_2             ((uint16_t)0x0001)
#define SDRAM_MODEREG_BURST_LENGTH_4             ((uint16_t)0x0002)
#define SDRAM_MODEREG_BURST_LENGTH_8             ((uint16_t)0x0004)
#define SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL      ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_TYPE_INTERLEAVED     ((uint16_t)0x0008)
#define SDRAM_MODEREG_CAS_LATENCY_2              ((uint16_t)0x0020)
#define SDRAM_MODEREG_CAS_LATENCY_3              ((uint16_t)0x0030)
#define SDRAM_MODEREG_OPERATING_MODE_STANDARD    ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_PROGRAMMED ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_SINGLE     ((uint16_t)0x0200)

void sdram_init(void);
void sdram_test(void);

#endif