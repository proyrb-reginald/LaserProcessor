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
    // uint16_t *p_buf_0 = lcd_buf_0;
    // for (uint32_t i = 0; i < LCD_BUF_SIZE; i++)
    // {
    //     *p_buf_0++ = value;
    // }
    // while (!(LTDC->CDSR & LTDC_CDSR_VSYNCS))
    // {
    //     DELAY_MS_INTERFACE(1);
    // }

    DMA2D->CR &= ~(DMA2D_CR_START); /* 先停止DMA2D */
    DMA2D->CR = DMA2D_R2M;          /* 寄存器到存储器模式 */
    // DMA2D->CR = DMA2D_M2M;          /* 存储器到存储器模式 */
    // DMA2D->OPFCCR = DMA2D_OUTPUT_RGB565; /* 设置颜色格式 */
    DMA2D->OOR = 0; /* 设置行偏移  */

    uint32_t pey = 800;
    uint32_t psy = 0;
    uint32_t pex = 1280;
    uint32_t psx = 0;
    DMA2D->OMAR = (uint32_t)lcd_buf_0;                      /* 输出存储器地址 */
    DMA2D->NLR = (pey - psy + 1) | ((pex - psx + 1) << 16); /* 设定行数寄存器 */
    DMA2D->OCOLR = value;        /* 设定输出颜色寄存器 */
    DMA2D->CR |= DMA2D_CR_START; /* 启动DMA2D */

    while ((DMA2D->ISR & (DMA2D_FLAG_TC)) == 0) /* 等待传输完成 */
    {
        DELAY_MS_INTERFACE(1);
    }
    DMA2D->IFCR |= DMA2D_FLAG_TC; /* 清除传输完成标志 */
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
        DELAY_MS_INTERFACE(33);
    }
}
#endif