#ifndef __OLED_SPI_V0_2_H
#define __OLED_SPI_V0_2_H

#include "ti_msp_dl_config.h"

#define OLED_CMD 0  // 鍐欏懡浠?
#define OLED_DATA 1 // 鍐欐暟鎹?
#define OLED_MODE 0

#define  u8  unsigned char 
#define  u32 unsigned int 


//-----------------OLED绔彛瀹氫箟----------------
// 瑕佹槸SPI鍚嶅瓧璁剧殑涓嶄竴鏍峰湪杩欐敼涓嬪氨琛?,渚嬪涓嬮潰杩欐牱
//#define SPI_OLED_INST SPI_1_INST

//// Using external CS(3-Wire Structure);
//#define OLED_CS_Clr()  DL_GPIO_clearPins(OLED_PORT,OLED_CS_PIN)
//#define OLED_CS_Set()  DL_GPIO_setPins(OLED_PORT,OLED_CS_PIN)

//纭欢SPI Syscofig 璁剧疆 SPI0
//				PICO only;
//				CLK; SPI
//				PICO; SPI
//				CS; SPI
//				DC; GPIO
//				RST; GPIO
//浠绘剰IO
//#define OLED_DC_Clr() DL_GPIO_clearPins(OLED_PORT, OLED_DC_PIN)
//#define OLED_DC_Set() DL_GPIO_setPins(OLED_PORT, OLED_DC_PIN)

#define SPI_OLED_INST SPI_0_INST

#define OLED_DC_Clr() DL_GPIO_clearPins(OLED_DC_PORT, OLED_DC_PIN)
#define OLED_DC_Set() DL_GPIO_setPins(OLED_DC_PORT, OLED_DC_PIN)

//浠绘剰IO
#define OLED_RST_Clr()  DL_GPIO_clearPins(OLED_RES_PORT, OLED_RES_PIN)
#define OLED_RST_Set()  DL_GPIO_setPins(OLED_RES_PORT, OLED_RES_PIN)

//// PB3
#define OLED_CS_Clr()  DL_GPIO_clearPins(OLED_CS_PORT,OLED_CS_PIN)
#define OLED_CS_Set()  DL_GPIO_setPins(OLED_CS_PORT,OLED_CS_PIN)


// OLED妯″紡璁剧疆
// 0:4绾夸覆琛屾ā寮?
// 1:骞惰8080妯″紡

#define CPU_Frq 80000	//Unit:kHz
#define SIZE 16
#define XLevelL 0x02
#define XLevelH 0x10
#define Max_Column 128
#define Max_Row 64
#define Brightness 0xFF
#define X_WIDTH 128
#define Y_WIDTH 64
#if (SIZE == 16)
#define YOFFSET 1
#else
#define YOFFSET 0
#endif

// OLED鎺у埗鐢ㄥ嚱鏁?
void OLED_WR_Byte(u8 dat, u8 cmd);
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x, u8 y, u8 t);
void OLED_Fill(u8 x1, u8 y1, u8 x2, u8 y2, u8 dot);
void OLED_ShowChar(u8 x, u8 y, char chr);
void OLED_ShowChar_small(u8 x, u8 y, char chr);
void OLED_ShowNum(u8 x, u8 y, u32 num, u8 len, u8 size2);
void OLED_ShowSignedNum(u8 x, u8 y, int32_t num, u8 len, u8 size2);
void OLED_ShowString(u8 x, u8 y, char* p);
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_ShowCHinese(u8 x, u8 y, u8 no);
void OLED_ShowCHinese_small(u8 x, u8 y, u8 no);
void OLED_DrawBMP(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, const unsigned char BMP[]);
void OLED_ColorTurn(u8 i);
void OLED_DisplayTurn(u8 i);
void OLED_Refresh(void);
void OLED_DrawPoint(u8 x, u8 y, u8 t);
void OLED_DrawLine(u8 x1, u8 y1, u8 x2, u8 y2, u8 mode);
void OLED_DrawCircle(u8 x, u8 y, u8 r);





////PA11
//#define OLED_DC_Clr() DL_GPIO_clearPins(OLED_DC_PORT, OLED_DC_PIN)
//#define OLED_DC_Set() DL_GPIO_setPins(OLED_DC_PORT, OLED_DC_PIN)



//#define OLED_SCLK_Clr() DL_GPIO_clearPins(OLED_DC_PORT,OLED_SCLK_PIN)
//#define OLED_SCLK_Set() DL_GPIO_setPins(OLED_DC_PORT,OLED_SCLK_PIN )

//#define OLED_SDIN_Clr() DL_GPIO_clearPins(OLED_DC_PORT,OLED_SDIN_PIN)
//#define OLED_SDIN_Set() DL_GPIO_setPins(OLED_DC_PORT,OLED_SDIN_PIN )


////OLED妯″紡璁剧疆
////0:4绾夸覆琛屾ā寮?
////1:骞惰8080妯″紡

//#define SIZE 16
//#define XLevelL		0x02
//#define XLevelH		0x10
//#define Max_Column	128
//#define Max_Row		64
//#define	Brightness	0xFF 
//#define X_WIDTH 	128
//#define Y_WIDTH 	64	    						  
////-----------------OLED绔彛瀹氫箟----------------
// 		     

////OLED鎺у埗鐢ㄥ嚱鏁?
//void OLED_WR_Byte(u8 dat,u8 cmd);	    
//void OLED_Display_On(void);
//void OLED_Display_Off(void);	   							   		    
//void OLED_Init(void);
//void OLED_Clear(void);
//void OLED_DrawPoint(u8 x,u8 y,u8 t);
//void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
//void OLED_ShowChar(u8 x,u8 y,u8 chr);
//void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size2);
//void OLED_ShowString(u8 x,u8 y, u8 *p);	 
//void OLED_Set_Pos(unsigned char x, unsigned char y);
//void OLED_ShowCHinese(u8 x,u8 y,u8 no);
//void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);
//void OLED_Show_int32(u8 x,u8 y,int32_t num,u8 len,u8 size2);
//void OLED_Show_float(u8 x,u8 y,float num,u8 len_1,u8 len_2);
#endif
