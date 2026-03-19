#ifndef INTERFACE_H
#define INTERFACE_H
#include "ti_msp_dl_config.h"
#include "oled_spi_V0.2.h"
#include "function.h"

void boot_screen();
void choice_screen();
void frame_screen();
void prompt_screen();
void pause_screen(p snake[], p food, int len);

#endif