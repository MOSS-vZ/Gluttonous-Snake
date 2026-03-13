#include "oled_spi_V0.2.h"
#include "oledfont.h"
#include <string.h>
// 鍚慡SD1306鍐欏叆涓?涓瓧鑺傘??
// dat:瑕佸啓鍏ョ殑鏁版嵁/鍛戒护
// cmd:鏁版嵁/鍛戒护鏍囧織 0,琛ㄧず鍛戒护;1,琛ㄧず鏁版嵁;
u8 OLED_GRAM[130][8];

//void OLED_DrawOneLine(u8 *x,u8 y)
//{
//	for (u8 i = 0; i < 8; i++)
//            OLED_WR_Byte(F8X16[c * 16 + i], OLED_DATA);
//}

void OLED_WR_Byte(u8 dat, u8 cmd)
{

    if (cmd)
    {
        OLED_DC_Set();
    }
    else
    {
        OLED_DC_Clr();
    }
    OLED_CS_Clr();
    while (DL_SPI_isBusy(SPI_OLED_INST)) {
    }
    DL_SPI_transmitData8(SPI_OLED_INST, dat);
    while (DL_SPI_isBusy(SPI_OLED_INST)) {
    }
    OLED_CS_Set();
    OLED_DC_Set();
}

void OLED_Set_Pos(unsigned char x, unsigned char y)
{
    OLED_WR_Byte(0xaf + y + YOFFSET, OLED_CMD);
    OLED_WR_Byte(((x & 0xf0) >> 4) | 0x10, OLED_CMD);
    OLED_WR_Byte((x & 0x0f) | 0x01, OLED_CMD);
}
// 寮?鍚疧LED鏄剧ず
void OLED_Display_On(void)
{
    OLED_WR_Byte(0X8D, OLED_CMD); // SET DCDC鍛戒护
    OLED_WR_Byte(0X14, OLED_CMD); // DCDC ON
    OLED_WR_Byte(0XAF, OLED_CMD); // DISPLAY ON
}
// 鍏抽棴OLED鏄剧ず
void OLED_Display_Off(void)
{
    OLED_WR_Byte(0X8D, OLED_CMD); // SET DCDC鍛戒护
    OLED_WR_Byte(0X10, OLED_CMD); // DCDC OFF
    OLED_WR_Byte(0XAE, OLED_CMD); // DISPLAY OFF
}
// 娓呭睆鍑芥暟,娓呭畬灞?,鏁翠釜灞忓箷鏄粦鑹茬殑!鍜屾病鐐逛寒涓?鏍?!!!
void OLED_Clear(void)
{
    u8 i, n;
    for (i = 0; i < 8; i++)
    {
        OLED_WR_Byte(0xb0 + i, OLED_CMD); // 璁剧疆椤靛湴鍧?锛?0~7锛?
        OLED_WR_Byte(0x02, OLED_CMD);     // 璁剧疆鏄剧ず浣嶇疆鈥斿垪浣庡湴鍧?
        OLED_WR_Byte(0x10, OLED_CMD);     // 璁剧疆鏄剧ず浣嶇疆鈥斿垪楂樺湴鍧?
        for (n = 0; n < 128; n++)
            OLED_WR_Byte(0, OLED_DATA);
    } // 鏇存柊鏄剧ず
    memset(OLED_GRAM, 0, sizeof(OLED_GRAM));
}

// 鍦ㄦ寚瀹氫綅缃樉绀轰竴涓瓧绗?,鍖呮嫭閮ㄥ垎瀛楃
// x:0~127
// y:0~7 (椤靛湴鍧?)
void OLED_ShowChar(u8 x, u8 y, char chr)
{
    unsigned char c = 0, i = 0;
    c = chr - ' '; // 寰楀埌鍋忕Щ鍚庣殑鍊?
    if (x > Max_Column - 1)
    {
        x = 0;
        y = y + 2;
    }
    if (SIZE == 16)
    {
        OLED_Set_Pos(x, y);
        for (i = 0; i < 8; i++)
            OLED_WR_Byte(F8X16[c * 16 + i], OLED_DATA);
        OLED_Set_Pos(x, y + 1);
        for (i = 0; i < 8; i++)
            OLED_WR_Byte(F8X16[c * 16 + i + 8], OLED_DATA);
    }
    else
    {
        OLED_Set_Pos(x, y + 1);
        for (i = 0; i < 6; i++)
            OLED_WR_Byte(F6x8[c][i], OLED_DATA);
    }
}

void OLED_ShowChar_small(u8 x, u8 y, char chr)
{
    unsigned char c = chr - ' ';  // 字库偏移
    if (y >= 8) return;           // 页必须 0~7
    for (u8 i = 0; i < 6; i++)
    {
        if (x + i < 128)
            OLED_GRAM[x + i][y] = F6x8[c][i];  // 写入页 y
    }
}
// m^n鍑芥暟
u32 oled_pow(u8 m, u8 n)
{
    u32 result = 1;
    while (n--)
        result *= m;
    return result;
}
// 鏄剧ず鏁板瓧
// x,y :璧风偣鍧愭爣
// len :鏁板瓧鐨勪綅鏁?
// size2:瀛椾綋澶у皬
// num:鏁板??(0~4294967295);
void OLED_ShowNum(u8 x, u8 y, u32 num, u8 len, u8 size2)
{
    // u8 t, temp;
    // u8 enshow = 0;
    // for (t = 0; t < len; t++)
    // {
    //     temp = (num / oled_pow(10, len - t - 1)) % 10;
    //     if (enshow == 0 && t < (len - 1))
    //     {
    //         if (temp == 0)
    //         {
    //             OLED_ShowChar(x + (size2 / 2) * t, y, ' ');
    //             continue;
    //         }
    //         else
    //             enshow = 1;
    //     }
    //     OLED_ShowChar(x + (size2 / 2) * t, y, temp + '0');
    // }

    u8 t, temp;
    u8 enshow = 0;
    u8 step;  // 字符宽度步进

    if (size2 == 8)
        step = 6;          // 小字体宽度6
    else
        step = size2 / 2;  // 大字体宽度（假设高度/2）

    for (t = 0; t < len; t++)
    {
        temp = (num / oled_pow(10, len - t - 1)) % 10;
        if (enshow == 0 && t < (len - 1))
        {
            if (temp == 0)
            {
                // 显示空格
                if (size2 == 8)
                    OLED_ShowChar_small(x + step * t, y, ' ');
                else
                    OLED_ShowChar(x + step * t, y, ' ');
                continue;
            }
            else
                enshow = 1;
        }
        // 显示数字字符
        if (size2 == 8)
            OLED_ShowChar_small(x + step * t, y, temp + '0');
        else
            OLED_ShowChar(x + step * t, y, temp + '0');
    }
}

void OLED_ShowSignedNum(u8 x, u8 y, int32_t num, u8 len, u8 size2)//鍙互鏄剧ず璐熸暟
{
    u8 t, temp;
    u8 enshow = 0;
    u8 isNegative = 0;

    // 鍒ゆ柇鏄惁涓鸿礋鏁?
    if (num < 0)
    {
        isNegative = 1;
        num = -num;  // 杞崲涓烘鏁拌繘琛屽鐞?
    }

    // 鏄剧ず璐熷彿
    if (isNegative)
    {
        OLED_ShowChar(x, y, '-');
        x += size2 / 2;  // 鏇存柊璧风偣鍧愭爣
        len--;  // 鍑忓皯涓?浣嶉暱搴︾敤浜庢樉绀鸿礋鍙?
    }

    for (t = 0; t < len; t++)
    {
        temp = (num / oled_pow(10, len - t - 1)) % 10;
        if (enshow == 0 && t < (len - 1))
        {
            if (temp == 0)
            {
                OLED_ShowChar(x + (size2 / 2) * t, y, ' ');
                continue;
            }
            else
                enshow = 1;
        }
        OLED_ShowChar(x + (size2 / 2) * t, y, temp + '0');
    }
}
// 鏄剧ず涓?涓瓧绗﹀彿涓?
void OLED_ShowString(u8 x, u8 y, char* chr)
{
    unsigned char j = 0;
    while (chr[j] != '\0')
    {
        OLED_ShowChar(x, y, chr[j]);
        x += 8;
        if (x > 120)
        {
            x = 0;
            y += 2;
        }
        j++;
    }
}
// 鏄剧ず姹夊瓧
void OLED_ShowCHinese(u8 x, u8 y, u8 no)
{
    u8 t, adder = 0;
    OLED_Set_Pos(x, y);
    for (t = 0; t < 16; t++)
    {
        OLED_WR_Byte(Hzk[2 * no][t], OLED_DATA);
        adder += 1;
    }
    OLED_Set_Pos(x, y + 1);
    for (t = 0; t < 16; t++)
    {
        OLED_WR_Byte(Hzk[2 * no + 1][t], OLED_DATA);
        adder += 1;
    }
}

void OLED_ShowCHinese_small(u8 x, u8 y, u8 no)
{
    u8 t, adder = 0;
    OLED_Set_Pos(x, y);
    for (t = 0; t < 8; t++)
    {
        // OLED_WR_Byte(Hzk_small[no][t], OLED_DATA);
        if (x + t < 128) // 防止列越界
            OLED_GRAM[x + t][y] = Hzk_small[no][t];
        adder += 1;
    }
}
/***********鍔熻兘鎻忚堪锛氭樉绀烘樉绀築MP鍥剧墖128脳64璧峰鐐瑰潗鏍?(x,y),x鐨勮寖鍥?0锝?127锛寉涓洪渶瑕佷娇鐢ㄧ殑椤电殑鑼冨洿1锝?8*****************/
void OLED_DrawBMP(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, const unsigned char BMP[])
{
    unsigned int j = 0;
    unsigned char x, y;

    if (y1 % 8 == 0)
        y = y1 / 8;
    else
        y = y1 / 8 + 1;
    for (y = y0; y < y1; y++)
    {
        OLED_Set_Pos(x0, y + (!YOFFSET));
        for (x = x0; x < x1; x++)
        {
            OLED_WR_Byte(BMP[j++], OLED_DATA);
        }
    }
}
//·′??oˉêy
void OLED_ColorTurn(u8 i)
{
    if (i == 0)
    {
        OLED_WR_Byte(0xA6, OLED_CMD);//?y3￡??ê?
    }
    if (i == 1)
    {
        OLED_WR_Byte(0xA7, OLED_CMD);//·′é???ê?
    }
}

//?á??Dy×a180?è
void OLED_DisplayTurn(u8 i)
{
    if (i == 0)
    {
        OLED_WR_Byte(0xC8, OLED_CMD);//?y3￡??ê?
        OLED_WR_Byte(0xA1, OLED_CMD);
    }
    if (i == 1)
    {
        OLED_WR_Byte(0xC0, OLED_CMD);//·′×a??ê?
        OLED_WR_Byte(0xA0, OLED_CMD);
    }
}
//?üD???′?μ?OLED	
void OLED_Refresh(void)
{
    u8 i, n;
    for (i = 0;i < 8;i++)
    {
        OLED_WR_Byte(0xb0 + i, OLED_CMD); //éè??DD?eê?μ??·
        OLED_WR_Byte(0x00, OLED_CMD);   //éè??μíáD?eê?μ??·
        OLED_WR_Byte(0x10, OLED_CMD);   //éè????áD?eê?μ??·
        OLED_CS_Clr();
        delay_cycles(CPU_Frq * 5);
        while (DL_SPI_isBusy(SPI_OLED_INST)) {}
        //        DL_SPI_transmitData8(SPI_OLED_INST, 0x78);
        //        while (DL_SPI_isBusy(SPI_OLED_INST)){}
        //		DL_SPI_transmitData8(SPI_OLED_INST, 0x78);
        //        while (DL_SPI_isBusy(SPI_OLED_INST)){}
        //		DL_SPI_transmitData8(SPI_OLED_INST, 0x40);
        while (DL_SPI_isBusy(SPI_OLED_INST)) {}
        for (n = 0;n < 128;n++) {
            DL_SPI_transmitData8(SPI_OLED_INST, OLED_GRAM[n][i]);
            // Send_Byte(OLED_GRAM[n][i]);
            while (DL_SPI_isBusy(SPI_OLED_INST)) {}
        }
        delay_cycles(CPU_Frq * 5);
        OLED_CS_Set();
        // I2C_Stop();
    }
}
//?-μ? 
//x:0~127
//y:0~63
//t:1 ì?3? 0,????	
void OLED_DrawPoint(u8 x, u8 y, u8 t)
{
    u8 i, m, n;
    i = y / 8;
    m = y % 8;
    n = 1 << m;
    if (t) { OLED_GRAM[x][i] |= n; }
    else
    {
        OLED_GRAM[x][i] = ~OLED_GRAM[x][i];
        OLED_GRAM[x][i] |= n;
        OLED_GRAM[x][i] = ~OLED_GRAM[x][i];
    }
}

//?-??
//x1,y1:?eμ?×?±ê
//x2,y2:?áê?×?±ê
void OLED_DrawLine(u8 x1, u8 y1, u8 x2, u8 y2, u8 mode)
{
    uint16_t t;
    int xerr = 0, yerr = 0, delta_x, delta_y, distance;
    int incx, incy, uRow, uCol;
    delta_x = x2 - x1; //????×?±ê??á? 
    delta_y = y2 - y1;
    uRow = x1;//?-???eμ?×?±ê
    uCol = y1;
    if (delta_x > 0)incx = 1; //éè??μ￥2?·??ò 
    else if (delta_x == 0)incx = 0;//′1?±?? 
    else { incx = -1;delta_x = -delta_x; }
    if (delta_y > 0)incy = 1;
    else if (delta_y == 0)incy = 0;//?????? 
    else { incy = -1;delta_y = -delta_x; }
    if (delta_x > delta_y)distance = delta_x; //??è??ù±???á?×?±ê?á 
    else distance = delta_y;
    for (t = 0;t < distance + 1;t++)
    {
        OLED_DrawPoint(uRow, uCol, mode);//?-μ?
        xerr += delta_x;
        yerr += delta_y;
        if (xerr > distance)
        {
            xerr -= distance;
            uRow += incx;
        }
        if (yerr > distance)
        {
            yerr -= distance;
            uCol += incy;
        }
    }
}
//x,y:?2D?×?±ê
//r:?2μ?°???
void OLED_DrawCircle(u8 x, u8 y, u8 r)
{
    int a, b, num;
    a = 0;
    b = r;
    while (2 * b * b >= r * r)
    {
        OLED_DrawPoint(x + a, y - b, 1);
        OLED_DrawPoint(x - a, y - b, 1);
        OLED_DrawPoint(x - a, y + b, 1);
        OLED_DrawPoint(x + a, y + b, 1);

        OLED_DrawPoint(x + b, y + a, 1);
        OLED_DrawPoint(x + b, y - a, 1);
        OLED_DrawPoint(x - b, y - a, 1);
        OLED_DrawPoint(x - b, y + a, 1);

        a++;
        num = (a * a + b * b) - r * r;//?????-μ?μ?à??2D?μ??àà?
        if (num > 0)
        {
            b--;
            a--;
        }
    }
}

// 鍒濆鍖朣SD1306
void OLED_Init(void)
{

    OLED_RST_Set();
    delay_cycles(CPU_Frq * 100);
    OLED_RST_Clr();
    delay_cycles(CPU_Frq * 100);
    OLED_RST_Set();

    OLED_WR_Byte(0xAE, OLED_CMD);//--turn off oled panel
    OLED_WR_Byte(0x02, OLED_CMD);//---set low column address
    OLED_WR_Byte(0x10, OLED_CMD);//---set high column address
    OLED_WR_Byte(0x40, OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
    OLED_WR_Byte(0x81, OLED_CMD);//--set contrast control register
    OLED_WR_Byte(0xCF, OLED_CMD); // Set SEG Output Current Brightness
    OLED_WR_Byte(0xA1, OLED_CMD);//--Set SEG/Column Mapping     0xa0宸﹀彸鍙嶇疆 0xa1姝ｅ父
    OLED_WR_Byte(0xC8, OLED_CMD);//Set COM/Row Scan Direction   0xc0涓婁笅鍙嶇疆 0xc8姝ｅ父
    OLED_WR_Byte(0xA6, OLED_CMD);//--set normal display
    OLED_WR_Byte(0xA8, OLED_CMD);//--set multiplex ratio(1 to 64)
    OLED_WR_Byte(0x3f, OLED_CMD);//--1/64 duty
    OLED_WR_Byte(0xD3, OLED_CMD);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
    OLED_WR_Byte(0x00, OLED_CMD);//-not offset
    OLED_WR_Byte(0xd5, OLED_CMD);//--set display clock divide ratio/oscillator frequency
    OLED_WR_Byte(0x80, OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
    OLED_WR_Byte(0xD9, OLED_CMD);//--set pre-charge period
    OLED_WR_Byte(0xF1, OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    OLED_WR_Byte(0xDA, OLED_CMD);//--set com pins hardware configuration
    OLED_WR_Byte(0x12, OLED_CMD);
    OLED_WR_Byte(0xDB, OLED_CMD);//--set vcomh
    OLED_WR_Byte(0x40, OLED_CMD);//Set VCOM Deselect Level
    OLED_WR_Byte(0x20, OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
    OLED_WR_Byte(0x02, OLED_CMD);//
    OLED_WR_Byte(0x8D, OLED_CMD);//--set Charge Pump enable/disable
    OLED_WR_Byte(0x14, OLED_CMD);//--set(0x10) disable
    OLED_WR_Byte(0xA4, OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
    OLED_WR_Byte(0xA6, OLED_CMD);// Disable Inverse Display On (0xa6/a7) 
    OLED_WR_Byte(0xAF, OLED_CMD);//--turn on oled panel

    OLED_WR_Byte(0xAF, OLED_CMD); /*display ON*/
    OLED_Clear();
    OLED_Set_Pos(0, 0);

    //    OLED_WR_Byte(0xAE, OLED_CMD); //--turn off oled panel
    //    OLED_WR_Byte(0x02, OLED_CMD); //---set low column address
    //    OLED_WR_Byte(0x10, OLED_CMD); //---set high column address
    //    OLED_WR_Byte(0x40, OLED_CMD); //--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
    //    OLED_WR_Byte(0x81, OLED_CMD); //--set contrast control register
    //    OLED_WR_Byte(0xCF, OLED_CMD); // Set SEG Output Current Brightness
    //    OLED_WR_Byte(0xA1, OLED_CMD); //--Set SEG/Column Mapping     0xa0宸﹀彸鍙嶇疆 0xa1姝ｅ父
    //    OLED_WR_Byte(0xC8, OLED_CMD); // Set COM/Row Scan Direction   0xc0涓婁笅鍙嶇疆 0xc8姝ｅ父
    //    OLED_WR_Byte(0xA6, OLED_CMD); //--set normal display
    //    OLED_WR_Byte(0xA8, OLED_CMD); //--set multiplex ratio(1 to 64)
    //    OLED_WR_Byte(0x3f, OLED_CMD); //--1/64 duty
    //    OLED_WR_Byte(0xD3, OLED_CMD); //-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
    //    OLED_WR_Byte(0x00, OLED_CMD); //-not offset
    //    OLED_WR_Byte(0xd5, OLED_CMD); //--set display clock divide ratio/oscillator frequency
    //    OLED_WR_Byte(0x80, OLED_CMD); //--set divide ratio, Set Clock as 100 Frames/Sec
    //    OLED_WR_Byte(0xD9, OLED_CMD); //--set pre-charge period
    //    OLED_WR_Byte(0xF1, OLED_CMD); // Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    //    OLED_WR_Byte(0xDA, OLED_CMD); //--set com pins hardware configuration
    //    OLED_WR_Byte(0x12, OLED_CMD);
    //    OLED_WR_Byte(0xDB, OLED_CMD); //--set vcomh
    //    OLED_WR_Byte(0x40, OLED_CMD); // Set VCOM Deselect Level
    //    OLED_WR_Byte(0x20, OLED_CMD); //-Set Page Addressing Mode (0x00/0x01/0x02)
    //    OLED_WR_Byte(0x02, OLED_CMD); //
    //    OLED_WR_Byte(0x8D, OLED_CMD); //--set Charge Pump enable/disable
    //    OLED_WR_Byte(0x14, OLED_CMD); //--set(0x10) disable
    //    OLED_WR_Byte(0xA4, OLED_CMD); // Disable Entire Display On (0xa4/0xa5)
    //    OLED_WR_Byte(0xA6, OLED_CMD); // Disable Inverse Display On (0xa6/a7)
    //    OLED_WR_Byte(0xAF, OLED_CMD); //--turn on oled panel

    //    OLED_WR_Byte(0xAF, OLED_CMD); /*display ON*/
    //    OLED_Clear();
    //    OLED_Set_Pos(0, 0);
}


//#if OLED_MODE==1
////鍚慡SD1106鍐欏叆涓?涓瓧鑺傘??
////dat:瑕佸啓鍏ョ殑鏁版嵁/鍛戒护
////cmd:鏁版嵁/鍛戒护鏍囧織 0,琛ㄧず鍛戒护;1,琛ㄧず鏁版嵁;
//void OLED_WR_Byte(u8 dat,u8 cmd)
//{
//	DATAOUT(dat);	    
//	if(cmd)
//	  OLED_DC_Set();
//	else 
//	  OLED_DC_Clr();		   
//	OLED_CS_Clr();
//	OLED_WR_Clr();	 
//	OLED_WR_Set();
//	OLED_CS_Set();	  
//	OLED_DC_Set();	 
//} 	    	    
//#else
////鍚慡SD1306鍐欏叆涓?涓瓧鑺傘??
////dat:瑕佸啓鍏ョ殑鏁版嵁/鍛戒护
////cmd:鏁版嵁/鍛戒护鏍囧織 0,琛ㄧず鍛戒护;1,琛ㄧず鏁版嵁;
//void OLED_WR_Byte(u8 dat,u8 cmd)
//{	
//	u8 i;			  
//	if(cmd)
//	  OLED_DC_Set();
//	else 
//	  OLED_DC_Clr();		  
//	OLED_CS_Clr();
//	for(i=0;i<8;i++)
//	{			  
//		OLED_SCLK_Clr();
//		if(dat&0x80)
//			{
//		   OLED_SDIN_Set();
//			}
//else
//		   OLED_SDIN_Clr();
//				OLED_SCLK_Set();
//		dat<<=1;   
//	}				 		  
//	OLED_CS_Set();
//	OLED_DC_Set();   	  
//} 
//#endif
//	void OLED_Set_Pos(unsigned char x, unsigned char y) 
//{ 
//	OLED_WR_Byte(0xb0+y,OLED_CMD);
//	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
//	OLED_WR_Byte((x&0x0f)|0x01,OLED_CMD); 
//}   	  
////寮?鍚疧LED鏄剧ず
//void OLED_Display_On(void)
//{
//	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC鍛戒护
//	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
//	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
//}
////鍏抽棴OLED鏄剧ず
//void OLED_Display_Off(void)
//{
//	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC鍛戒护
//	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
//	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
//}		   			 
////娓呭睆鍑芥暟,娓呭畬灞?,鏁翠釜灞忓箷鏄粦鑹茬殑!鍜屾病鐐逛寒涓?鏍?!!!
//void OLED_Clear(void)  
//{  
//	u8 i,n;		    
//	for(i=0;i<8;i++)  
//	{  
//		OLED_WR_Byte (0xb0+i,OLED_CMD);    //璁剧疆椤靛湴鍧?锛?0~7锛?
//		OLED_WR_Byte (0x02,OLED_CMD);      //璁剧疆鏄剧ず浣嶇疆鈥斿垪浣庡湴鍧?
//		OLED_WR_Byte (0x10,OLED_CMD);      //璁剧疆鏄剧ず浣嶇疆鈥斿垪楂樺湴鍧?
//		for(n=0;n<128;n++)OLED_WR_Byte(0,OLED_DATA); 
//	} //鏇存柊鏄剧ず
//}


////鍦ㄦ寚瀹氫綅缃樉绀轰竴涓瓧绗?,鍖呮嫭閮ㄥ垎瀛楃
////x:0~127
////y:0~63
////mode:0,鍙嶇櫧鏄剧ず;1,姝ｅ父鏄剧ず
////size:閫夋嫨瀛椾綋 16/12
//void OLED_ShowChar(u8 x,u8 y,u8 chr)
//{      	
//	unsigned char c=0,i=0;	
//		c=chr-' ';//寰楀埌鍋忕Щ鍚庣殑鍊?
//		if(x>Max_Column-1){x=0;y=y+2;}
//		if(SIZE ==16)
//			{
//			OLED_Set_Pos(x,y);	
//			for(i=0;i<8;i++)
//			OLED_WR_Byte(F8X16[c*16+i],OLED_DATA);
//			OLED_Set_Pos(x,y+1);
//			for(i=0;i<8;i++)
//			OLED_WR_Byte(F8X16[c*16+i+8],OLED_DATA);
//			}
//			else {	
//				OLED_Set_Pos(x,y+1);
//				for(i=0;i<6;i++)
//				OLED_WR_Byte(F6x8[c][i],OLED_DATA);
//				
//			}
//}
////m^n鍑芥暟
//u32 oled_pow(u8 m,u8 n)
//{
//	u32 result=1;	 
//	while(n--)result*=m;    
//	return result;
//}				  
////鏄剧ず2涓暟瀛?
////x,y :璧风偣鍧愭爣
////len :鏁板瓧鐨勪綅鏁?
////size:瀛椾綋澶у皬
////mode:妯″紡	0,濉厖妯″紡;1,鍙犲姞妯″紡
////num:鏁板??(0~4294967295);
//void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size2)
//{         	
//	u8 t,temp;
//	u8 enshow=0;						   
//	for(t=0;t<len;t++)
//	{
//		temp=(num/oled_pow(10,len-t-1))%10;
//		if(enshow==0&&t<(len-1))
//		{
//			if(temp==0)
//			{
//				OLED_ShowChar(x+(size2/2)*t,y,' ');
//				continue;
//			}else enshow=1; 
//		 	 
//		}
//	 	OLED_ShowChar(x+(size2/2)*t,y,temp+'0'); 
//	}
//} 
////鏄剧ず涓?涓瓧绗﹀彿涓?
//void OLED_ShowString(u8 x,u8 y,u8 *chr)
//{
//	unsigned char j=0;
//	while (chr[j]!='\0')
//	{		OLED_ShowChar(x,y,chr[j]);
//			x+=8;
//		if(x>120){x=0;y+=2;}
//			j++;
//	}
//}
////鏄剧ず姹夊瓧
//void OLED_ShowCHinese(u8 x,u8 y,u8 no)
//{      			    
//	u8 t,adder=0;
//	OLED_Set_Pos(x,y);	
//    for(t=0;t<16;t++)
//		{
//				OLED_WR_Byte(Hzk[2*no][t],OLED_DATA);
//				adder+=1;
//     }	
//		OLED_Set_Pos(x,y+1);	
//    for(t=0;t<16;t++)
//			{	
//				OLED_WR_Byte(Hzk[2*no+1][t],OLED_DATA);
//				adder+=1;
//      }					
//}

//void OLED_Show_int32(u8 x,u8 y,int32_t num,u8 len,u8 size2)//瀹為檯鏄剧ず浣嶆暟姣旇緭鍏ヤ綅鏁板1锛岀敤浜庣鍙锋樉绀?
//{
//	
//	if(num<0)
//	{
//    OLED_ShowChar(x,y,'-');
//		OLED_ShowNum(x+8,y,-num,len,size2);	
//	}
//		else
//		{
//			OLED_ShowNum(x,y,num,len+1,size2);
//		}
//}
//void OLED_Show_float(u8 x,u8 y,float num,u8 len_1,u8 len_2)//len_1涓烘暣鏁颁綅鏁帮紝len_2涓哄皬鏁颁綅鏁?,瀹為檯鏄剧ず浣嶆暟涓簂en_1+len_2+2
//{
//	if(num<0)
//	{
//    OLED_ShowChar(x,y,'-');
//	  OLED_ShowNum(x+8,y,-num,len_1,16);
//    OLED_ShowChar(x+len_1*8+8,y,'.');
//	  OLED_ShowNum(x+len_1*8+16,y,-(num-(int32_t)num)*oled_pow(10,len_2),len_2,16);
//	}
//		else
//		{			
//			OLED_ShowNum(x,y,num,len_1+1,16);
//			OLED_ShowChar(x+len_1*8+8,y,'.');
//			OLED_ShowNum(x+len_1*8+16,y,(num-(int32_t)num)*oled_pow(10,len_2),len_2,16);
//		}
//}
///***********鍔熻兘鎻忚堪锛氭樉绀烘樉绀築MP鍥剧墖128脳64璧峰鐐瑰潗鏍?(x,y),x鐨勮寖鍥?0锝?127锛寉涓洪〉鐨勮寖鍥?0锝?7*****************/
//void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[])
//{ 	
// unsigned int j=0;
// unsigned char x,y;
//  
//  if(y1%8==0) y=y1/8;      
//  else y=y1/8+1;
//	for(y=y0;y<y1;y++)
//	{
//		OLED_Set_Pos(x0,y);
//    for(x=x0;x<x1;x++)
//	    {      
//	    	OLED_WR_Byte(BMP[j++],OLED_DATA);	    	
//	    }
//	}
//} 


////鍒濆鍖朣SD1306
//void OLED_Init(void)
//{
// 

// 
//  OLED_RST_Set();
//	system_delay_ms(100);
//	OLED_RST_Clr();
//	system_delay_ms(100);
//	OLED_RST_Set(); 
//	/*				  
//	OLED_WR_Byte(0xAE,OLED_CMD);//--turn off oled panel
//	OLED_WR_Byte(0x00,OLED_CMD);//---set low column address
//	OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
//	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
//	OLED_WR_Byte(0x81,OLED_CMD);//--set contrast control register
//	OLED_WR_Byte(0xCF,OLED_CMD); // Set SEG Output Current Brightness
//	OLED_WR_Byte(0xA1,OLED_CMD);//--Set SEG/Column Mapping     0xa0宸﹀彸鍙嶇疆 0xa1姝ｅ父
//	OLED_WR_Byte(0xC8,OLED_CMD);//Set COM/Row Scan Direction   0xc0涓婁笅鍙嶇疆 0xc8姝ｅ父
//	OLED_WR_Byte(0xA6,OLED_CMD);//--set normal display
//	OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
//	OLED_WR_Byte(0x3f,OLED_CMD);//--1/64 duty
//	OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
//	OLED_WR_Byte(0x00,OLED_CMD);//-not offset
//	OLED_WR_Byte(0xd5,OLED_CMD);//--set display clock divide ratio/oscillator frequency
//	OLED_WR_Byte(0x80,OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
//	OLED_WR_Byte(0xD9,OLED_CMD);//--set pre-charge period
//	OLED_WR_Byte(0xF1,OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
//	OLED_WR_Byte(0xDA,OLED_CMD);//--set com pins hardware configuration
//	OLED_WR_Byte(0x12,OLED_CMD);
//	OLED_WR_Byte(0xDB,OLED_CMD);//--set vcomh
//	OLED_WR_Byte(0x40,OLED_CMD);//Set VCOM Deselect Level
//	OLED_WR_Byte(0x20,OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
//	OLED_WR_Byte(0x02,OLED_CMD);//
//	OLED_WR_Byte(0x8D,OLED_CMD);//--set Charge Pump enable/disable
//	OLED_WR_Byte(0x14,OLED_CMD);//--set(0x10) disable
//	OLED_WR_Byte(0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
//	OLED_WR_Byte(0xA6,OLED_CMD);// Disable Inverse Display On (0xa6/a7) 
//	OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel
//	*/

//	OLED_WR_Byte(0xAE,OLED_CMD);//--turn off oled panel
//	OLED_WR_Byte(0x02,OLED_CMD);//---set low column address
//	OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
//	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
//	OLED_WR_Byte(0x81,OLED_CMD);//--set contrast control register
//	OLED_WR_Byte(0xCF,OLED_CMD); // Set SEG Output Current Brightness
//	OLED_WR_Byte(0xA1,OLED_CMD);//--Set SEG/Column Mapping     0xa0宸﹀彸鍙嶇疆 0xa1姝ｅ父
//	OLED_WR_Byte(0xC8,OLED_CMD);//Set COM/Row Scan Direction   0xc0涓婁笅鍙嶇疆 0xc8姝ｅ父
//	OLED_WR_Byte(0xA6,OLED_CMD);//--set normal display
//	OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
//	OLED_WR_Byte(0x3f,OLED_CMD);//--1/64 duty
//	OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
//	OLED_WR_Byte(0x00,OLED_CMD);//-not offset
//	OLED_WR_Byte(0xd5,OLED_CMD);//--set display clock divide ratio/oscillator frequency
//	OLED_WR_Byte(0x80,OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
//	OLED_WR_Byte(0xD9,OLED_CMD);//--set pre-charge period
//	OLED_WR_Byte(0xF1,OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
//	OLED_WR_Byte(0xDA,OLED_CMD);//--set com pins hardware configuration
//	OLED_WR_Byte(0x12,OLED_CMD);
//	OLED_WR_Byte(0xDB,OLED_CMD);//--set vcomh
//	OLED_WR_Byte(0x40,OLED_CMD);//Set VCOM Deselect Level
//	OLED_WR_Byte(0x20,OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
//	OLED_WR_Byte(0x02,OLED_CMD);//
//	OLED_WR_Byte(0x8D,OLED_CMD);//--set Charge Pump enable/disable
//	OLED_WR_Byte(0x14,OLED_CMD);//--set(0x10) disable
//	OLED_WR_Byte(0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
//	OLED_WR_Byte(0xA6,OLED_CMD);// Disable Inverse Display On (0xa6/a7) 
//	OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel
//	
//	OLED_WR_Byte(0xAF,OLED_CMD); /*display ON*/ 
//	OLED_Clear();
//	OLED_Set_Pos(0,0); 	
//}  
