#include "bsp.h"

volatile uint8_t received_byte;

void UART_remote_INST_IRQHandler(void)
{
    switch (DL_UART_getPendingInterrupt(UART_remote_INST))
    {
    case DL_UART_IIDX_RX: // 接收中断
        received_byte = DL_UART_Main_receiveData(UART_remote_INST);
        break;
    default:
        break;
    }
}

int main()
{
    SYSCFG_DL_init();
    OLED_Init();

    srand(seed);   //初始化种子（随机数）
    // DL_GPIO_clearPins(LED_L1_PORT, LED_L1_PIN);
    NVIC_EnableIRQ(TIMER_0_INST_INT_IRQN);//开启定时器中断
    NVIC_EnableIRQ(UART_remote_INST_INT_IRQN);//开启UART中断

    boot_screen();
    delay_cycles(64000000);
    OLED_Clear();

    bool is_key = 0;
    bool is_first = 1;
    while (1)
    {
        choice_screen();
        if (is_first)
        {
            OLED_Refresh();
            is_first = 0;
        }

        char key = KeySCInput();
        if (key && !is_key)
        {
            is_key = 1;
            switch (key)
            {
            case 'A':
                game();
                OLED_Clear();
                is_first = 1;
                break;
            case 'B':
                key_interface();
                OLED_Clear();
            default:
                break;
            }

        }
        else if (key == '\0')
            is_key = 0;

        switch (received_byte)
        {
        case 'A':
            received_byte = '\0';
            game();
            OLED_Clear();
            is_first = 1;
            break;
        case 'B':
            received_byte = '\0';
            key_interface();
            OLED_Clear();
        default:
            break;
        }
    }
}