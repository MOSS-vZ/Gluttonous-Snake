#include "function.h"
#include "ti_msp_dl_config.h"
#include "interface.h"
#include "Keyboard.h"
#include <stdlib.h>
#include <time.h>

uint8_t len = initial_length;
p snake[100];//定义贪吃蛇
p food = { 0, 0 };//定义食物
bool is_over = 0;//结束标志
bool is_time = 0;//定时器标志
uint8_t direction = 0;
uint8_t score = 0;
// uint8_t time = 0;

void game()
{
    // DL_GPIO_setPins(LED_L1_PORT, LED_L1_PIN);

    OLED_Clear();
    snake_init();//初始化
    frame_screen();
    prompt_screen();

    bool is_key = 0;
    while (!is_over)
    {
        if (!is_time)
        {
            food_generator();
            is_time = 1;
        }
        move();
        if (is_eat())
        {
            len++;
            score += 5;
            prompt_screen();
            DL_Timer_stopCounter(TIMER_0_INST);//停止定时器
            is_time = 0;
        }
        char key = KeySCInput();
        if (key && !is_key)
        {
            is_key = 1;
            switch (key)
            {
            case '#':
                DL_Timer_stopCounter(TIMER_0_INST);//停止定时器
                pause_screen(snake, food, len);
                DL_Timer_startCounter(TIMER_0_INST);//启动定时器
                break;
            case '*':
                is_over = 1;
                break;
            case '2':case '8':case '4':case '6':
                direction = key - '0';
                break;
            default:
                break;
            }
        }
        else
            is_key = 0;
    }
    is_over = 0;
    score = 0;
    direction = 0;
    len = initial_length;
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
    frame_screen();
    //边界判断
    if (snake[0].x < left_boundary || snake[0].x >= right_boundary || snake[0].y < upper_boundary || snake[0].y >= lower_boundary)
    {
        score -= 2;
        prompt_screen();
        // DL_GPIO_togglePins(LED_L1_PORT, LED_L1_PIN);
    }

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
    // 清除旧食物
    if (food.x != 0 || food.y != 0)
    {
        OLED_DrawPoint(food.x + 1, food.y, 0);
        OLED_DrawPoint(food.x - 1, food.y, 0);
        OLED_DrawPoint(food.x, food.y + 1, 0);
        OLED_DrawPoint(food.x, food.y - 1, 0);
    }

    // 生成不重叠的新食物
    uint8_t x, y;
    do {
        x = random_range(left_boundary + 1, right_boundary - 1);
        y = random_range(upper_boundary + 1, lower_boundary - 1);
    } while (is_food_on_snake(x, y));

    food.x = x;
    food.y = y;
    OLED_DrawPoint(x + 1, y, 1);
    OLED_DrawPoint(x - 1, y, 1);
    OLED_DrawPoint(x, y + 1, 1);
    OLED_DrawPoint(x, y - 1, 1);
    OLED_Refresh();

    DL_Timer_startCounter(TIMER_0_INST);//启动定时器
}

uint8_t random_range(uint8_t min, uint8_t max)
{
    return min + rand() % (max - min + 1);
}

bool is_eat()
{
    uint8_t x = food.x;
    uint8_t y = food.y;
    if ((GetPoint(x + 1, y) == 1) || (GetPoint(x - 1, y) == 1) || (GetPoint(x, y + 1) == 1) || (GetPoint(x, y - 1) == 1))
        return 1;
    else
        return 0;
}

bool GetPoint(uint8_t x, uint8_t y)
{
    if (snake[0].x == x && snake[0].y == y)
        return 1;
    else
        return 0;
}

bool is_food_on_snake(uint8_t x, uint8_t y)
{
    // 食物的五个关键点：中心、右、左、下、上
    uint8_t points[5][2] = {
        {x, y},       // 中心点（虽未绘制，但逻辑上占据该位置）
        {x + 1, y},   // 右
        {x - 1, y},   // 左
        {x, y + 1},   // 下
        {x, y - 1}    // 上
    };

    // 遍历每个点，检查是否与蛇的任何一节重叠
    for (uint8_t i = 0; i < 5; i++) {
        uint8_t px = points[i][0];
        uint8_t py = points[i][1];

        // 如果该点超出游戏边界，则不可能有蛇身（假设蛇始终在边界内）
        if (px < left_boundary || px > right_boundary ||
            py < upper_boundary || py > lower_boundary) {
            continue;   // 跳过越界点
        }

        for (uint8_t j = 0; j < len; j++) {
            if (snake[j].x == px && snake[j].y == py) {
                return true;   // 发现重叠
            }
        }
    }
    return false;  // 无重叠
}

void TIMER_0_INST_IRQHandler()
{
    is_time = 0;
}