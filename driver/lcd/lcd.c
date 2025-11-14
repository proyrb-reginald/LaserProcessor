/* 导入接口层 */
#include HAL_INC  // 硬件抽象层
#include BSP_INC  // 板机支持层
#include RTOS_INC // 实时操作系统层

#if USE_TEST
#define LCD_BUF_SIZE ((uint32_t)((800 * 1280)))
static
    __attribute__((section(".sdram.lcd.0"))) uint16_t lcd_buf_0[LCD_BUF_SIZE];
static
    __attribute__((section(".sdram.lcd.1"))) uint16_t lcd_buf_1[LCD_BUF_SIZE];

void lcd_fill(uint16_t value)
{
    uint16_t *p_buf_0 = lcd_buf_0;
    for (uint32_t i = 0; i < LCD_BUF_SIZE; i++)
    {
        *p_buf_0++ = value;
    }
    while (!(LTDC->CDSR & LTDC_CDSR_VSYNCS))
    {
        DELAY_MS_INTERFACE(1);
    }
}

void lcd_test(void)
{
    while (1)
    {
        static uint8_t cnt = 0;
        if (cnt++ % 2 == 0)
        {
            lcd_fill(0xF800);
        }
        else
        {
            lcd_fill(0x001F);
        }
        DELAY_MS_INTERFACE(3000);
    }
}
#endif