#include <board.h>
#include <rtthread.h>
#include <sdram.h>

void thread_test(void *parameter)
{
    sdram_init(); // SDRAM设备初始化
    while (1)
    {
        sdram_test();
    }
}

int main(void)
{
    // 创建并启动温度控制线程
    rt_thread_t ctr_tid =
        rt_thread_create("test", thread_test, RT_NULL, 1024, 3, 0);
    if (ctr_tid != RT_NULL)
    {
        if (rt_thread_startup(ctr_tid) == RT_EOK)
        {
            rt_kprintf("thread[%s] start finish.\n", ctr_tid->name);
        }
        else
        {
            rt_kprintf("thread[%s] start fail.\n", ctr_tid->name);
            rt_thread_delete(ctr_tid);
        }
    }
    else
    {
        rt_kprintf("thread create fail\n");
    }

    while (1)
    {
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_1);
        rt_thread_mdelay(500);
    }
}