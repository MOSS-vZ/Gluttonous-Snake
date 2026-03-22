#ifndef INTERFACE_H
#define INTERFACE_H

#include "bsp.h"

void boot_screen();
void choice_screen();
void frame_screen();
void prompt_screen();
void pause_screen(p snake[], p food, int len);
void key_interface();

#endif