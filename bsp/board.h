#ifndef BOARD_H
#define BOARD_H

/* 导入接口层 */
#include HAL_INC // 硬件抽象层
#include <main.h>
#include <gpio.h>
#include <usart.h>
#include <fmc.h>
#include <rthw.h>
#include <rtthread.h>
#include <sdram.h>
#include <ltdc.h>

extern int rt_lprintf(const char *fmt, ...);

#endif