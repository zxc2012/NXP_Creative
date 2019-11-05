#ifndef	_LCD_H_
#define     _LCD_H_

#include "headfile.h"

/********用户引脚定义********
**只需在此修改相应的引脚即可
**     D0=SCL        D1=SDA
*****************************/
/*
#define  SCL_0         hw_gpio_init(PORT_A, 17, 1,0)  gpio_init (PTA17, 1,0);
#define  SCL_1         hw_gpio_init(PORT_A, 17, 1,1)   gpio_init (PTA17, 1,1);
#define  SDA_0         hw_gpio_init(PORT_A, 16, 1,0)   gpio_init (PTA16, 1,0);
#define  SDA_1         hw_gpio_init(PORT_A, 16, 1,1)     gpio_init (PTA16, 1,1);
#define  DC_data       hw_gpio_init(PORT_A, 14, 1,1)      gpio_init (PTA14, 1,1);
#define  DC_cmd      hw_gpio_init(PORT_A, 14, 1,0)         gpio_init (PTA14, 1,0);
#define  REST_0       hw_gpio_init(PORT_A, 15, 1,0)         gpio_init (PTA15, 1,0);
#define  REST_1       hw_gpio_init(PORT_A, 15, 1,1)           gpio_init (PTA15, 1,1);
//#define  CS_0        备用引脚
//#define  CS_1        备用引脚
#define  SCL_FIRST         hw_gpio_init(PORT_A, 17, 1,0)     gpio_init (PTA17, 1,0);
#define  SDA_FIRST         hw_gpio_init(PORT_A, 16, 1,0)      gpio_init (PTA16, 1,0);
#define  DC_FIRST           hw_gpio_init(PORT_A, 14, 1,1)        gpio_init (PTA14, 1,1);
#define  REST_FIRST       hw_gpio_init(PORT_A, 15, 1,0)             gpio_init (PTA15, 1,0);
//#define  CS_FIRST        备用引脚
*/


// #define OLED_SCL  A8  //D0
//#define OLED_SDA  A7  //D1
//#define OLED_RST  A6
//#define OLED_DC   A5

#define SCL_PORT     A8         //D0
#define SDA_PORT     A7         //D1
#define RESET_PORT   A6
#define DC_PORT      A5

#define  SCL_0                 gpio_set (SCL_PORT, 0);
#define  SCL_1                 gpio_set (SCL_PORT, 1);
#define  SDA_0                 gpio_set (SDA_PORT, 0);
#define  SDA_1                 gpio_set (SDA_PORT, 1);
#define  DC_data               gpio_set (DC_PORT,  1);
#define  DC_cmd                gpio_set (DC_PORT,  0);
#define  REST_0                gpio_set (RESET_PORT,0);
#define  REST_1                gpio_set (RESET_PORT,1);
//#define  CS_0                备用引脚
//#define  CS_1                备用引脚
#define  SCL_FIRST             gpio_init (SCL_PORT, 1,0);
#define  SDA_FIRST             gpio_init (SDA_PORT, 1,0);
#define  DC_FIRST              gpio_init (DC_PORT, 1,1);
#define  REST_FIRST            gpio_init (RESET_PORT, 1,1);






/****************************/
#define byte uint8
#define word uint16

 extern byte beyond64x64_1[512];
 extern byte beyond64x64_2[512];
 void LCD_Init(void);
 void LCD_CLS(void);
 void LCD_P6x8Str(byte x,byte y,byte ch[]);
 void LCD_P8x16Str(byte x,byte y,byte ch[]);
 void LCD_PutPixel(byte x,byte y);
 void LCD_Rectangle(byte x1,byte y1,byte x2,byte y2,byte gif);
 extern void LCD_Print(unsigned char x, unsigned char y, unsigned char ch[]);
 void LCD_Fill(byte dat);
 void P_LCD_CLS(byte x,byte y);
//extern void Img_Show();
//extern void Line_Show();
extern void OLED_Num(unsigned char x,unsigned char y,unsigned char asc);
extern void OLED_3num(unsigned char x,unsigned char y,unsigned char number);
extern void OLED_4num(unsigned char x,unsigned char y, int number);
//extern void OLED_Num5(unsigned char x,unsigned char y,unsigned int number)
#endif