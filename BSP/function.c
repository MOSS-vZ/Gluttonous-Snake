#include "function.h"
#include "ti_msp_dl_config.h"
#include "oled_spi_V0.2.h"
#include "Keyboard.h"
#include <stdlib.h>
#include <time.h>

uint8_t len = initial_length;
p snake[initial_length];//定义贪吃蛇
bool is_over = 0;//结束标志
uint8_t direction = 0;

void game()
{
    OLED_Clear();
    snake_init();//初始化

    bool is_key = 0;
    while (!is_over)
    {
        move();
        char key = KeySCInput();
        if (key && !is_key)
        {
            is_key = 1;
            direction = key - '0';
        }
        else
            is_key = 0;
    }
}

void move()
{
    //擦除蛇尾
    OLED_DrawPoint(snake[len - 1].x, snake[len - 1].y, 0);
    //将身体前移
    for (uint8_t i = len - 1;i > 0;i--)
        snake[i] = snake[i - 1];
    //改变蛇头方向
    switch (direction)
    {
    case 0:case 4:
        snake[0].x -= 1;
        break;
    case 2:
        snake[0].y -= 1;
        break;
    case 6:
        snake[0].x += 1;
        break;
    case 8:
        snake[0].y += 1;
        break;
    default:
        break;
    }

    snake_refresh(len);
}

void snake_init()
{
    snake[0].x = 40;
    snake[0].y = 40;

    for (int i = 1;i < initial_length;i++)
    {
        snake[i].x = snake[i - 1].x + 1;
        snake[i].y = snake[i - 1].y;
    }
    snake_refresh(initial_length);
}

void snake_refresh(uint8_t length)
{
    //边界判断
    if (snake[0].x < left_boundary)
        snake[0].x = right_boundary;
    if (snake[0].x > right_boundary)
        snake[0].x = left_boundary;
    if (snake[0].y < upper_boundary)
        snake[0].y = lower_boundary;
    if (snake[0].y > lower_boundary)
        snake[0].y = upper_boundary;
    //点亮
    for (int j = 0;j < length;j++)
    {
        OLED_DrawPoint(snake[j].x, snake[j].y, 1);
    }
    OLED_Refresh();
}

void food_generator()
{
    uint8_t x = random_range(left_boundary, right_boundary);
    uint8_t y = random_range(upper_boundary, lower_boundary);
    OLED_DrawPoint(x + 1, y, 1);
    OLED_DrawPoint(x - 1, y, 1);
    OLED_DrawPoint(x, y + 1, 1);
    OLED_DrawPoint(x, y - 1, 1);
    OLED_Refresh();
}

uint8_t random_range(uint8_t min, uint8_t max)
{
    return min + rand() % (max - min + 1);
}