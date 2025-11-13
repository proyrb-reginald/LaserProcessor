#include <sdram.h>

#define SDRAM_TEST_SIZE 16

#define SDRAM_MODEREG_BURST_LENGTH_1 ((SDRAM_UINT16_TYPE)0x0000)
#define SDRAM_MODEREG_BURST_LENGTH_2 ((SDRAM_UINT16_TYPE)0x0001)
#define SDRAM_MODEREG_BURST_LENGTH_4 ((SDRAM_UINT16_TYPE)0x0002)
#define SDRAM_MODEREG_BURST_LENGTH_8 ((SDRAM_UINT16_TYPE)0x0004)
#define SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL ((SDRAM_UINT16_TYPE)0x0000)
#define SDRAM_MODEREG_BURST_TYPE_INTERLEAVED ((SDRAM_UINT16_TYPE)0x0008)
#define SDRAM_MODEREG_CAS_LATENCY_2 ((SDRAM_UINT16_TYPE)0x0020)
#define SDRAM_MODEREG_CAS_LATENCY_3 ((SDRAM_UINT16_TYPE)0x0030)
#define SDRAM_MODEREG_OPERATING_MODE_STANDARD ((SDRAM_UINT16_TYPE)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_PROGRAMMED ((SDRAM_UINT16_TYPE)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_SINGLE ((SDRAM_UINT16_TYPE)0x0200)

void sdram_clear(void)
{
    SDRAM_UINT8_TYPE *pSRAM = (SDRAM_UINT8_TYPE *)SDRAM_ADDR;
    for (SDRAM_UINT32_TYPE i = 0; i < SDRAM_SIZE; i++)
    {
        *pSRAM++ = 0;
    }
}

void sdram_init(void)
{
    /* 时钟使能命令 */
    sdram_send_cmd(FMC_SDRAM_CMD_CLK_ENABLE, 1, 1, 0);
    SDRAM_DELAY_INTERFACE(1);

    /* SDRAM全部预充电命令 */
    sdram_send_cmd(FMC_SDRAM_CMD_PALL, 1, 1, 0);

    /* 自动刷新命令 */
    sdram_send_cmd(FMC_SDRAM_CMD_AUTOREFRESH_MODE, 1, 8, 0);

    /* 配置SDRAM模式寄存器 */
    SDRAM_UINT32_TYPE temp;
    temp = (SDRAM_UINT32_TYPE)SDRAM_MODEREG_BURST_LENGTH_1 | // 设置突发长度：1
           SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL |   // 设置突发类型：连续
           SDRAM_MODEREG_CAS_LATENCY_3 |           // 设置CL值：3
           SDRAM_MODEREG_OPERATING_MODE_STANDARD | // 设置操作模式：标准
           SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;   // 设置突发写模式：单点访问
    sdram_send_cmd(FMC_SDRAM_CMD_LOAD_MODE, 1, 1, temp);

    /* 设置自刷新频率 */
    /* SDRAM refresh period / Number of rows）*SDRAM时钟速度 – 20
     * = 64000(64ms) / 8192 * 120MHz - 20
     */
    HAL_SDRAM_ProgramRefreshRate(&hsdram1, 918);

    /* 清空SDRAM */
    sdram_clear();
}

void sdram_test(void)
{
    SDRAM_UINT8_TYPE r_data[SDRAM_TEST_SIZE] = {0};
    SDRAM_UINT8_TYPE t_data[SDRAM_TEST_SIZE] = {0};
    for (SDRAM_UINT32_TYPE i = 0; i < sizeof(t_data); i++)
    {
        t_data[i] = i;
    }

    /* 读SDRAM */
    SDRAM_LOG_INTERFACE("first read after boot\n");
    sdram_read_data(0, r_data, sizeof(r_data));
    for (SDRAM_UINT32_TYPE i = 0; i < sizeof(r_data); i++)
    {
        SDRAM_LOG_INTERFACE("[0x%X:0x%X]\n", i, r_data[i]);
    }

    /* 写SDRAM */
    SDRAM_LOG_INTERFACE("write fix value\n");
    sdram_write_data(0, t_data, sizeof(t_data));

    /* 读SDRAM */
    SDRAM_LOG_INTERFACE("read after write\n");
    sdram_read_data(0, r_data, sizeof(r_data));
    for (SDRAM_UINT32_TYPE i = 0; i < sizeof(r_data); i++)
    {
        SDRAM_LOG_INTERFACE("[0x%X:0x%X]\n", i, r_data[i]);
    }

    /* 对SDRAM的数据求反并写入 */
    SDRAM_LOG_INTERFACE("write rebit value\n");
    for (SDRAM_UINT32_TYPE i = 0; i < sizeof(t_data); i++)
    {
        t_data[i] = ~t_data[i];
    }
    sdram_write_data(0, t_data, sizeof(t_data));

    /* 再次比较SDRAM的数据 */
    SDRAM_LOG_INTERFACE("read after rebit\n");
    sdram_read_data(0, r_data, sizeof(r_data));
    for (SDRAM_UINT32_TYPE i = 0; i < sizeof(r_data); i++)
    {
        SDRAM_LOG_INTERFACE("[0x%X:0x%X]\n", i, r_data[i]);
    }
}