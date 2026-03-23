#include "bsp.h"

void boot_screen()
{
    OLED_ShowCHinese(45, 3, 27);
    OLED_ShowCHinese(61, 3, 28);
    OLED_ShowCHinese(77, 3, 29);
}

void choice_screen()
{
    OLED_ShowCHinese(0, 0, 32);
    OLED_ShowCHinese(16, 0, 33);
    OLED_ShowCHinese(32, 0, 34);
    OLED_ShowCHinese(48, 0, 35);

    OLED_ShowCHinese(96, 0, 36);
    OLED_ShowChar(112, 0, 'A');

    OLED_ShowCHinese(0, 2, 45);
    OLED_ShowCHinese(16, 2, 46);
    OLED_ShowCHinese(32, 2, 47);
    OLED_ShowCHinese(48, 2, 48);

    OLED_ShowCHinese(96, 2, 36);
    OLED_ShowChar(112, 2, 'B');
}

void frame_screen()
{
    OLED_DrawLine(left_boundary, upper_boundary, right_boundary, upper_boundary, 1);
    OLED_DrawLine(left_boundary, lower_boundary, right_boundary, lower_boundary, 1);
    OLED_DrawLine(left_boundary, upper_boundary, left_boundary, lower_boundary, 1);
    OLED_DrawLine(right_boundary, upper_boundary, right_boundary, lower_boundary, 1);
}

void prompt_screen()
{
    OLED_ShowCHinese_small(2, 0, 0);
    OLED_ShowCHinese_small(18, 0, 1);

    OLED_ShowCHinese_small(80, 0, 2);
    OLED_ShowCHinese_small(96, 0, 3);

    OLED_ShowNum(110, 0, score, 3, 8);

    OLED_ShowCHinese_small(2, 15, 4);
    OLED_ShowChar_small(12, 7, '*');
    OLED_ShowCHinese_small(18, 15, 5);
    OLED_ShowCHinese_small(26, 15, 6);

    OLED_ShowCHinese_small(80, 15, 4);
    OLED_ShowChar_small(90, 7, '#');
    OLED_ShowCHinese_small(96, 15, 7);
    OLED_ShowCHinese_small(104, 15, 8);
    OLED_Refresh();
}

void pause_screen(p snake[], p food, int len)
{
    // 将蛇和食物点灭
    for (int i = 0;i < len;i++)
    {
        OLED_DrawPoint(snake[i].x, snake[i].y, 0);
    }
    OLED_DrawPoint(food.x + 1, food.y, 0);
    OLED_DrawPoint(food.x - 1, food.y, 0);
    OLED_DrawPoint(food.x, food.y + 1, 0);
    OLED_DrawPoint(food.x, food.y - 1, 0);
    OLED_Refresh();

    // 显示暂停界面
    OLED_ShowCHinese(30, 2, 34);
    OLED_ShowCHinese(46, 2, 35);
    OLED_ShowCHinese(62, 2, 39);
    OLED_ShowCHinese(78, 2, 40);

    OLED_ShowCHinese(30, 4, 37);
    OLED_ShowCHinese(46, 4, 38);
    OLED_ShowCHinese(62, 4, 36);
    OLED_ShowChar(82, 4, '#');

    bool is_key = 1;
    bool is_over = 0;
    while (!is_over)
    {
        char key = KeySCInput();
        if (key && is_key)
        {
            is_key = 0;
            switch (key)
            {
            case '#':
                // 将暂停界面点灭
                for (int i = left_boundary - 1;i < right_boundary;i++)
                {
                    for (int j = upper_boundary - 1;j < lower_boundary;j++)
                        OLED_DrawPoint(i, j, 0);
                }
                // 重新绘制蛇和食物
                for (int i = 0;i < len;i++)
                {
                    OLED_DrawPoint(snake[i].x, snake[i].y, 1);
                }
                OLED_DrawPoint(food.x + 1, food.y, 1);
                OLED_DrawPoint(food.x - 1, food.y, 1);
                OLED_DrawPoint(food.x, food.y + 1, 1);
                OLED_DrawPoint(food.x, food.y - 1, 1);
                OLED_Refresh();
                is_over = 1;
                break;
            default:
                break;
            }
        }
        else if (key == '\0')
        {
            is_key = 1;
        }

        switch (received_byte)
        {
        case '#':
            received_byte = '\0';
            // 将暂停界面点灭
            for (int i = left_boundary - 1;i < right_boundary;i++)
            {
                for (int j = upper_boundary - 1;j < lower_boundary;j++)
                    OLED_DrawPoint(i, j, 0);
            }
            // 重新绘制蛇和食物
            for (int i = 0;i < len;i++)
            {
                OLED_DrawPoint(snake[i].x, snake[i].y, 1);
            }
            OLED_DrawPoint(food.x + 1, food.y, 1);
            OLED_DrawPoint(food.x - 1, food.y, 1);
            OLED_DrawPoint(food.x, food.y + 1, 1);
            OLED_DrawPoint(food.x, food.y - 1, 1);
            OLED_Refresh();
            is_over = 1;
            break;
        default:
            break;
        }
    }
}

void key_interface()
{
    bool is_over = 0;

    OLED_Clear();

    OLED_ShowCHinese(0, 0, 41);
    OLED_ShowCHinese(0, 2, 42);
    OLED_ShowCHinese(70, 0, 43);
    OLED_ShowCHinese(70, 2, 44);

    for (int i = 18;i < 45;i += 9)
        for (int j = 0;j < 4;j += 2)
            OLED_ShowChar(i, j, '.');

    for (int i = 88;i < 115;i += 9)
        for (int j = 0;j < 4;j += 2)
            OLED_ShowChar(i, j, '.');

    OLED_ShowChar(45, 0, '2');
    OLED_ShowChar(45, 2, '8');
    OLED_ShowChar(115, 0, '4');
    OLED_ShowChar(115, 2, '6');

    OLED_ShowCHinese(0, 6, 36);
    OLED_ShowChar(18, 6, '*');
    OLED_ShowCHinese(27, 6, 49);
    OLED_ShowCHinese(43, 6, 50);
    OLED_ShowCHinese(59, 6, 51);
    OLED_ShowCHinese(75, 6, 52);
    OLED_ShowCHinese(91, 6, 53);

    bool is_key = 1;
    while (!is_over)
    {
        char key = KeySCInput();
        if (key && is_key)
        {
            is_key = 0;
            switch (key)
            {
            case '*':
                is_over = 1;
                break;
            default:
                break;
            }
        }
        else if (key == '\0')
        {
            is_key = 1;
        }

        switch (received_byte)
        {
        case '*':
            received_byte = '\0';
            is_over = 1;
            break;
        default:
            break;
        }
    }
}