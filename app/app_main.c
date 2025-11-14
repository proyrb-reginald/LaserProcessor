#include <board.h>
#include <lcd.h>
#include <rtthread.h>
#include <sdram.h>

void thread_test(void *parameter)
{
    while (1)
    {
        // sdram_test_16b();
        lcd_test();
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
        HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
        HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
        DELAY_MS_INTERFACE(500);
    }
}