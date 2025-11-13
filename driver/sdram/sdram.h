#ifndef SDRAM_H
#define SDRAM_H

/* 初始化SDRAM设备 */
void sdram_init(void);

/* 清空SDRAM数据 */
void sdram_clear(void);

#if USE_TEST
void sdram_test(void);
#endif

#endif