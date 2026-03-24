#ifndef FUNCTION_H
#define FUNCTION_H

#include "bsp.h"

#define initial_length 20
#define left_boundary 2
#define right_boundary 127
#define upper_boundary 9
#define lower_boundary 54
#define seed 12345

extern uint8_t score;

typedef struct {
    uint8_t x;
    uint8_t y;
}p;

void game();
void move();
void snake_init();
bool is_eat();
void food_generator();
void snake_refresh(uint8_t length);
void TIMER_0_INST_IRQHandler();
bool is_food_on_snake(uint8_t x, uint8_t y);
bool GetPoint(uint8_t x, uint8_t y);
uint8_t random_range(uint8_t min, uint8_t max);
void death_animation();

#endif