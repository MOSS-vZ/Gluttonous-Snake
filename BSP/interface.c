#include "interface.h"
#include "oled_spi_V0.2.h"

void boot_screen()
{
    OLED_ShowCHinese(45, 3, 27);
    OLED_ShowCHinese(61, 3, 28);
    OLED_ShowCHinese(77, 3, 29);
}

void choice_screen()
{
    OLED_ShowCHinese(0, 1, 32);
    OLED_ShowCHinese(16, 1, 33);
    OLED_ShowCHinese(32, 1, 34);
    OLED_ShowCHinese(48, 1, 35);

    OLED_ShowCHinese(96, 1, 36);
    OLED_ShowChar(112, 1, 'A');
}