#ifndef FUNCTION_H
#define FUNCTION_H

#include "ti_msp_dl_config.h"

#define initial_length 20
#define left_boundary 1
#define right_boundary 126
#define upper_boundary 1
#define lower_boundary 62

typedef struct {
    uint8_t x;
    uint8_t y;
}p;

void game();
void move();
void snake_init();
void food_generator();
void snake_refresh(uint8_t length);
uint8_t random_range(uint8_t min, uint8_t max);

#endif