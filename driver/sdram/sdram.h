#ifndef SDRAM_H
#define SDRAM_H

/* 初始化SDRAM设备 */
void sdram_init(void);

/* 清空SDRAM数据 */
void sdram_clear(void);

#if USE_TEST
/* 测试SDRAM设备 */
void sdram_test_8b(void);
void sdram_test_16b(void);
#endif

#endif