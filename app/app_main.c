#include <board.h>
#include <rtthread.h>

int main(void)
{
    while (1)
    {
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_1);
        rt_lprintf("Hello RT-Thread!\n");
        rt_thread_mdelay(500);
    }
}