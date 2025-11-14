/* 导入接口层 */
#include HAL_INC  // 硬件抽象层
#include BSP_INC  // 板机支持层
#include RTOS_INC // 实时操作系统层

#include <sdram.h>

/* 定义SDRAM设备信息 */
#define SDRAM_ADDR ((uint32_t)0xC0000000)
#define SDRAM_SIZE ((uint32_t)(32 * 1024 * 1024))

/* 定义SDRAM可选参数 */
#define SDRAM_BURST_LEN_1 ((uint16_t)0x0000)
#define SDRAM_BURST_LEN_2 ((uint16_t)0x0001)
#define SDRAM_BURST_LEN_4 ((uint16_t)0x0002)
#define SDRAM_BURST_LEN_8 ((uint16_t)0x0004)
#define SDRAM_BURST_TYPE_SEQUENTIAL ((uint16_t)0x0000)
#define SDRAM_BURST_TYPE_INTERLEAVED ((uint16_t)0x0008)
#define SDRAM_CAS_LATENCY_2 ((uint16_t)0x0020)
#define SDRAM_CAS_LATENCY_3 ((uint16_t)0x0030)
#define SDRAM_OPERATING_MODE_STANDARD ((uint16_t)0x0000)
#define SDRAM_WRITEBURST_MODE_PROGRAMMED ((uint16_t)0x0000)
#define SDRAM_WRITEBURST_MODE_SINGLE ((uint16_t)0x0200)

/* SDRAM设备控制函数 */
static void sdram_send_cmd(uint32_t cmd_mode, uint32_t bank,
                           uint32_t refresh_cnt, uint32_t reg_val)
{
    FMC_SDRAM_CommandTypeDef cmd;
    cmd.CommandMode = cmd_mode;
    if (bank == 1)
    {
        cmd.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
    }
    else if (bank == 2)
    {
        cmd.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK2;
    }
    cmd.AutoRefreshNumber = refresh_cnt;
    cmd.ModeRegisterDefinition = reg_val;
    HAL_SDRAM_SendCommand(&hsdram1, &cmd, 10);
}

void sdram_init(void)
{
    /* 时钟使能命令 */
    sdram_send_cmd(FMC_SDRAM_CMD_CLK_ENABLE, 1, 1, 0);

    /* SDRAM全部预充电命令 */
    sdram_send_cmd(FMC_SDRAM_CMD_PALL, 1, 1, 0);

    /* 自动刷新命令 */
    sdram_send_cmd(FMC_SDRAM_CMD_AUTOREFRESH_MODE, 1, 8, 0);

    /* 配置SDRAM模式寄存器 */
    uint32_t temp;
    temp = (uint32_t)SDRAM_BURST_LEN_1 |   // 设置突发长度：1
           SDRAM_BURST_TYPE_SEQUENTIAL |   // 设置突发类型：连续
           SDRAM_CAS_LATENCY_3 |           // 设置CL值：3
           SDRAM_OPERATING_MODE_STANDARD | // 设置操作模式：标准
           SDRAM_WRITEBURST_MODE_SINGLE;   // 设置突发写模式：单点访问
    sdram_send_cmd(FMC_SDRAM_CMD_LOAD_MODE, 1, 1, temp);

    /* 设置自刷新频率 */
    /* SDRAM refresh period / Number of rows）*SDRAM时钟速度 – 20
     * = 64000(64ms) / 8192 * 120MHz - 20
     */
    HAL_SDRAM_ProgramRefreshRate(&hsdram1, 918);
}

void sdram_clear(void)
{
    uint8_t *p_sdram = (uint8_t *)SDRAM_ADDR;
    for (uint32_t i = 0; i < SDRAM_SIZE; i++)
    {
        *p_sdram++ = 0;
    }
}

void sdram_read_data(uint32_t rel_addr, uint8_t *buf, uint32_t buf_size)
{
    HAL_SDRAM_Read_8b(&hsdram1, (uint32_t *)(SDRAM_ADDR + rel_addr), buf,
                      buf_size);
}

void sdram_write_data(uint32_t rel_addr, uint8_t *buf, uint32_t buf_size)
{
    HAL_SDRAM_Write_8b(&hsdram1, (uint32_t *)(SDRAM_ADDR + rel_addr), buf,
                       buf_size);
}

#if USE_TEST
#define SDRAM_TEST_SIZE 1024

void sdram_test_8b(void)
{
    static __attribute__((
        section(".sdram.sdram"))) uint8_t sdram_data[SDRAM_TEST_SIZE];

    sdram_clear();

    LOG_INTERFACE("=====[first read]=====\n");
    // SCB_InvalidateDCache_by_Addr(sdram_data, SDRAM_TEST_SIZE);
    for (uint32_t i = 0; i < SDRAM_TEST_SIZE; i++)
    {
        LOG_INTERFACE("0x%X > 0x%X\n", &(sdram_data[i]), sdram_data[i]);
    }

    for (uint32_t i = 0; i < SDRAM_TEST_SIZE; i++)
    {
        sdram_data[i] = i;
    }
    // SCB_CleanDCache_by_Addr((uint32_t *)sdram_data, SDRAM_TEST_SIZE);

    LOG_INTERFACE("=====[read after write]=====\n");
    // SCB_InvalidateDCache_by_Addr(sdram_data, SDRAM_TEST_SIZE);
    for (uint32_t i = 0; i < SDRAM_TEST_SIZE; i++)
    {
        LOG_INTERFACE("0x%X > 0x%X\n", &(sdram_data[i]), sdram_data[i]);
    }

    rt_thread_mdelay(3000);

    LOG_INTERFACE("=====[read after 3s]=====\n");
    // SCB_InvalidateDCache_by_Addr(sdram_data, SDRAM_TEST_SIZE);
    for (uint32_t i = 0; i < SDRAM_TEST_SIZE; i++)
    {
        LOG_INTERFACE("0x%X > 0x%X\n", &(sdram_data[i]), sdram_data[i]);
    }

    while (1)
    {
        rt_thread_mdelay(1000);
    }
}

void sdram_test_16b(void)
{
    static __attribute__((
        section(".sdram.sdram"))) uint16_t sdram_data[SDRAM_TEST_SIZE];

    sdram_clear();

    LOG_INTERFACE("=====[first read]=====\n");
    // SCB_InvalidateDCache_by_Addr(sdram_data, SDRAM_TEST_SIZE);
    for (uint32_t i = 0; i < SDRAM_TEST_SIZE; i++)
    {
        LOG_INTERFACE("0x%X > 0x%X\n", &(sdram_data[i]), sdram_data[i]);
    }

    for (uint32_t i = 0; i < SDRAM_TEST_SIZE; i++)
    {
        sdram_data[i] = i;
    }
    // SCB_CleanDCache_by_Addr((uint32_t *)sdram_data, SDRAM_TEST_SIZE);

    LOG_INTERFACE("=====[read after write]=====\n");
    // SCB_InvalidateDCache_by_Addr(sdram_data, SDRAM_TEST_SIZE);
    for (uint32_t i = 0; i < SDRAM_TEST_SIZE; i++)
    {
        LOG_INTERFACE("0x%X > 0x%X\n", &(sdram_data[i]), sdram_data[i]);
    }

    rt_thread_mdelay(3000);

    LOG_INTERFACE("=====[read after 3s]=====\n");
    // SCB_InvalidateDCache_by_Addr(sdram_data, SDRAM_TEST_SIZE);
    for (uint32_t i = 0; i < SDRAM_TEST_SIZE; i++)
    {
        LOG_INTERFACE("0x%X > 0x%X\n", &(sdram_data[i]), sdram_data[i]);
    }

    while (1)
    {
        rt_thread_mdelay(1000);
    }
}
#endif