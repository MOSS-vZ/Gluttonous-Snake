#include "ti_msp_dl_config.h"
#include "oled_spi_V0.2.h"
#include "Keyboard.h"
#include "interface.h"
#include "function.h"
#include <stdlib.h>
#include <time.h>

int main()
{
    SYSCFG_DL_init();
    OLED_Init();

    srand(seed);   //初始化种子（随机数）
    // DL_GPIO_clearPins(LED_L1_PORT, LED_L1_PIN);
    NVIC_EnableIRQ(TIMER_0_INST_INT_IRQN);//开启定时器中断

    boot_screen();
    delay_cycles(64000000);
    OLED_Clear();
    choice_screen();

    bool is_key = 0;
    while (1)
    {
        char key = KeySCInput();
        if (key && !is_key)
        {
            is_key = 1;
            switch (key)
            {
            case 'A':
                game();
                break;

            default:
                break;
            }
        }
        else
            is_key = 0;
    }
}