/**
  ******************************************************************************
  * @file    gd32150r_eval_lcd.c
  * @author  MCU SD
  * @version V1.0.1
  * @date    6-Sep-2014
  * @brief   This file includes the LCD driver for AM-240320L8TNQW00H (LCD_ILI9320)
  *          Liquid Crystal Display Modules of GD32150R-EVAL board.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "gd32150r_eval_lcd.h"
#include "lcd_font.h"
#include "usb_hwp.h"

#ifdef USE_LCD_LOG

/** @addtogroup Utilities
  * @{
  */

/** @addtogroup GD32_EVAL
  * @{
  */ 

/** @addtogroup GD32150R_EVAL
  * @{
  */
    
/** @defgroup GD32150R_EVAL_LCD
  * @{
  */

/** @defgroup GD32150R_EVAL_LCD_Private_Defines
  * @{
  */
#ifdef H_VIEW
    #define X_MAX_PIXEL         320
    #define Y_MAX_PIXEL         240
#else
    #define X_MAX_PIXEL         240
    #define Y_MAX_PIXEL         320
#endif

/**
  * @}
  */

/** @defgroup GD32150R_EVAL_LCD_Private_Macros
  * @{
  */
static uint8_t SPI_WriteByte    (SPI_TypeDef* SPIx, uint8_t Byte);

static void LCD_WriteIndex      (uint8_t Index);
static void LCD_WriteData       (uint8_t Data);
static void LCD_Reset           (void);
static void SPI_LCD_Init        (void);
static void LCD_Initializtion   (void);
static void LCD_WriteData16Bit  (uint8_t DataH, uint8_t DataL);

/**
  * @}
  */

/** @defgroup GD32150R_EVAL_LCD_Private_Functions
  * @{
  */

/**
  * @brief  Write an unsigned 8-bit bytes.
  * @param  SPIx: SPI1 or SPI2
  * @param  Byte: writing byte
  * @retval None
  */
static uint8_t SPI_WriteByte(SPI_TypeDef* SPIx, uint8_t Byte)
{
    while((SPIx->STR & SPI_FLAG_TBE) == RESET);

    SPIx->DTR = Byte;

    while((SPIx->STR & SPI_FLAG_RBNE) == RESET);

    return SPIx->DTR;
} 

/**
  * @brief  Init SPI1.
  * @param  None
  * @retval None
  */
static void SPI_LCD_Init(void)
{
    SPI_InitPara SPI_InitStructure;

    /* Configure the GPIO ports */
    GPIO_InitPara GPIO_InitStructure;

    RCC_AHBPeriphClock_Enable(RCC_AHBPERIPH_GPIOB | RCC_AHBPERIPH_GPIOC | \
                              RCC_AHBPERIPH_GPIOF, ENABLE);

    /* Connect pin to Periph */
    GPIO_PinAFConfig(GPIOB, GPIO_PINSOURCE3, GPIO_AF_0);
    GPIO_PinAFConfig(GPIOB, GPIO_PINSOURCE4, GPIO_AF_0);
    GPIO_PinAFConfig(GPIOB, GPIO_PINSOURCE5, GPIO_AF_0);

    /* Configure pins as AF pushpull */
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_3 |
                                  GPIO_PIN_4 |
                                  GPIO_PIN_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_50MHZ;
    GPIO_InitStructure.GPIO_OType = GPIO_OTYPE_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PUPD_NOPULL;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_OUT;

    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_3 | GPIO_PIN_4;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_5;
    GPIO_Init(GPIOF, &GPIO_InitStructure);
    GPIO_SetBits(GPIOF, GPIO_PIN_5);

    RCC_APB2PeriphClock_Enable(RCC_APB2PERIPH_SPI1, ENABLE);

    /* Configure the SPI1 */
    SPI_InitStructure.SPI_TransType = SPI_TRANSTYPE_FULLDUPLEX;
    SPI_InitStructure.SPI_Mode = SPI_MODE_MASTER;
    SPI_InitStructure.SPI_FrameFormat = SPI_FRAMEFORMAT_8BIT;
    SPI_InitStructure.SPI_SCKPL = SPI_SCKPL_HIGH;
    SPI_InitStructure.SPI_SCKPH = SPI_SCKPH_2EDGE;
    SPI_InitStructure.SPI_SWNSSEN = SPI_SWNSS_SOFT;
    SPI_InitStructure.SPI_PSC = SPI_PSC_2;
    SPI_InitStructure.SPI_FirstBit = SPI_FIRSTBIT_MSB;
    SPI_InitStructure.SPI_CRCPOL = 7;
    SPI_Init(SPI1, &SPI_InitStructure);

    SPI_Enable(SPI1, ENABLE);
}

/**
  * @brief  Write the register address.
  * @param  Index: register address
  * @retval None
  */
static void LCD_WriteIndex(uint8_t Index)
{
    LCD_RS_CLR;

    SPI_WriteByte(SPI1,Index);
}

/**
  * @brief  Write the register data.
  * @param  Data: register data
  * @retval None
  */
static void LCD_WriteData(uint8_t Data)
{
    LCD_RS_SET;

    SPI_WriteByte(SPI1,Data);
}

/**
  * @brief  Write the register data(an unsigned 16-bit data).
  * @param  DataH: register data high byte
  * @param  DataL: register data low byte
  * @retval None
  */
static void LCD_WriteData16Bit(uint8_t DataH,uint8_t DataL)
{
    LCD_WriteData(DataH);
    LCD_WriteData(DataL);
}

/**
  * @brief  LCD reset.
  * @param  None
  * @retval None
  */
static void LCD_Reset(void)
{
    LCD_RST_CLR;

    USB_HWP_mDelay(100);

    LCD_RST_SET;

    USB_HWP_mDelay(50);
}

/**
  * @brief  LCD Init.
  * @param  None
  * @retval None
  */
static void LCD_Initializtion (void)
{
    LCD_CS_CLR;
    LCD_Reset();

    LCD_WriteIndex(0xCB);
    LCD_WriteData(0x39);
    LCD_WriteData(0x2C);
    LCD_WriteData(0x00);
    LCD_WriteData(0x34);
    LCD_WriteData(0x02);

    LCD_WriteIndex(0xCF);
    LCD_WriteData(0x00);
    LCD_WriteData(0xC1);
    LCD_WriteData(0X30);

    LCD_WriteIndex(0xE8);
    LCD_WriteData(0x85);
    LCD_WriteData(0x00);
    LCD_WriteData(0x78);

    LCD_WriteIndex(0xEA);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);

    LCD_WriteIndex(0xED);
    LCD_WriteData(0x64);
    LCD_WriteData(0x03);
    LCD_WriteData(0X12);
    LCD_WriteData(0X81);

    LCD_WriteIndex(0xF7);
    LCD_WriteData(0x20);

    /* Power control VRH[5:0] */
    LCD_WriteIndex(0xC0);
    LCD_WriteData(0x23);

    /* Power control SAP[2:0];BT[3:0] */
    LCD_WriteIndex(0xC1);
    LCD_WriteData(0x10);

    /* VCM control */
    LCD_WriteIndex(0xC5);
    LCD_WriteData(0x3e);
    LCD_WriteData(0x28); 

    /* VCM control2 */
    LCD_WriteIndex(0xC7);
    LCD_WriteData(0x86);

    LCD_WriteIndex(0x36);
#ifdef H_VIEW
    LCD_WriteData(0xE8);
#else
    LCD_WriteData(0x48); 
#endif

    LCD_WriteIndex(0x3A);
    LCD_WriteData(0x55); 

    LCD_WriteIndex(0xB1);
    LCD_WriteData(0x00);
    LCD_WriteData(0x18);

    /* Display Function Control */
    LCD_WriteIndex(0xB6);
    LCD_WriteData(0x08); 
    LCD_WriteData(0x82);
    LCD_WriteData(0x27);

    /* 3Gamma Function Disable */
    LCD_WriteIndex(0xF2);
    LCD_WriteData(0x00); 

    /* Gamma curve selected */
    LCD_WriteIndex(0x26);
    LCD_WriteData(0x01); 

    /* Set Gamma  */
    LCD_WriteIndex(0xE0);
    LCD_WriteData(0x0F);
    LCD_WriteData(0x31);
    LCD_WriteData(0x2B);
    LCD_WriteData(0x0C);
    LCD_WriteData(0x0E);
    LCD_WriteData(0x08);
    LCD_WriteData(0x4E);
    LCD_WriteData(0xF1);
    LCD_WriteData(0x37);
    LCD_WriteData(0x07);
    LCD_WriteData(0x10);
    LCD_WriteData(0x03);
    LCD_WriteData(0x0E);
    LCD_WriteData(0x09);
    LCD_WriteData(0x00);

    /* Set Gamma  */
    LCD_WriteIndex(0XE1);
    LCD_WriteData(0x00);
    LCD_WriteData(0x0E);
    LCD_WriteData(0x14);
    LCD_WriteData(0x03);
    LCD_WriteData(0x11);
    LCD_WriteData(0x07);
    LCD_WriteData(0x31);
    LCD_WriteData(0xC1);
    LCD_WriteData(0x48);
    LCD_WriteData(0x08);
    LCD_WriteData(0x0F);
    LCD_WriteData(0x0C);
    LCD_WriteData(0x31);
    LCD_WriteData(0x36);
    LCD_WriteData(0x0F);

    /* Exit Sleep */
    LCD_WriteIndex(0x11);
    USB_HWP_mDelay(120);

    /* Display on */
    LCD_WriteIndex(0x29);
    LCD_WriteIndex(0x2c);

    LCD_CS_SET;
}

/**
  * @brief  Initializes the LCD.
  * @param  None
  * @retval None
  */
void GD32150R_LCD_Init (void)
{
    /* Configure GPIO and SPI1 */
    SPI_LCD_Init();

    /* Configure LCD */
    LCD_Initializtion();
}

/**
  * @brief  Set LCD display region.
  * @param  StartX: the start row-coordinate
  * @param  StartY: the start column-coordinate
  * @param  EndX: the end row-coordinate
  * @param  EndY: the end column-coordinate
  * @retval None
  */
void LCD_SetRegion(uint16_t StartX, 
                   uint16_t StartY, 
                   uint16_t EndX, 
                   uint16_t EndY)
{
    LCD_CS_CLR;

    LCD_WriteIndex(0x2a);

    LCD_WriteData16Bit(StartX >> 8, StartX);
    LCD_WriteData16Bit(EndX >> 8, EndX);

    LCD_WriteIndex(0x2b);

    LCD_WriteData16Bit(StartY >> 8, StartY);
    LCD_WriteData16Bit(EndY >> 8, EndY);

    LCD_WriteIndex(0x2c);

    LCD_CS_SET;
}

/**
  * @brief  Set the start display point of lcd. 
  * @param  X: specifies the X position.
  * @param  Y: specifies the Y position.
  * @retval None
  */
void LCD_SetXY(uint16_t X, uint16_t Y)
{
    LCD_WriteIndex(0x2a);
    LCD_WriteData16Bit(X >> 8, X);

    LCD_WriteIndex(0x2b);
    LCD_WriteData16Bit(Y >> 8, Y);

    LCD_WriteIndex(0x2c);
}

/**
  * @brief  Draw a point on the LCD.
  * @param  X: the row-coordinate.
  * @param  Y: the column-coordinate.
  * @param  Data: specified the color of point
  * @retval None
  */
void LCD_DrawPoint(uint16_t X, uint16_t Y, uint16_t Data)
{
    LCD_SetXY(X, Y);

    LCD_WriteData(Data >> 8);
    LCD_WriteData(Data);
}

/**
  * @brief  Clear the LCD screen to the specified color.
  * @param  Color: specified the screen color.
  * @retval None
  */
void LCD_Clear(uint16_t Color)
{
    unsigned int i, j;

    LCD_SetRegion(0, 0, X_MAX_PIXEL - 1, Y_MAX_PIXEL - 1);
    LCD_RS_SET;

    LCD_CS_CLR;

    for(i = 0; i < Y_MAX_PIXEL; i++)
    {
        for(j = 0; j < X_MAX_PIXEL; j++)
        {
            SPI_WriteByte(SPI1, Color >> 8);
            SPI_WriteByte(SPI1, Color);
        }
    }

    LCD_CS_SET;
}

/**
  * @brief  GBR to RGB format conversion.
  * @param  Color: RGB colour value
  * @retval GBR color value.
  */
uint16_t LCD_BGR2RGB(uint16_t Color)
{
    uint16_t R, G, B, RGB;

    B = (Color >> 0) & 0x1f;
    G = (Color >> 5) & 0x3f;
    R = (Color >> 11) & 0x1f;

    RGB = (B << 11) + (G << 5) + (R << 0);

    return RGB;
}

/**
  * @brief  Draw a circle in LCD.
  * @param  X, Y: center of the circle
  * @param  R: radius of the circle
  * @param  fc: color of the circle
  * @retval None
  */
void LCD_DrawCircle(uint16_t X, 
                    uint16_t Y, 
                    uint16_t R, 
                    uint16_t fc)
{
    unsigned short a = 0, b = R;
    int c = 3 - 2 * R;

    LCD_CS_CLR;

    while (a < b)
    {
        LCD_DrawPoint(X + a, Y + b, fc);
        LCD_DrawPoint(X - a, Y + b, fc);
        LCD_DrawPoint(X + a, Y - b, fc);
        LCD_DrawPoint(X - a, Y - b, fc);
        LCD_DrawPoint(X + b, Y + a, fc);
        LCD_DrawPoint(X - b, Y + a, fc);
        LCD_DrawPoint(X + b, Y - a, fc);
        LCD_DrawPoint(X - b, Y - a, fc);

        if(c < 0) c = c + 4 * a + 6;
        else 
        { 
            c = c + 4 * (a - b) + 10;
            b -= 1; 
        }

        a += 1;
    }

    if (a == b)
    { 
        LCD_DrawPoint(X + a, Y + b, fc);
        LCD_DrawPoint(X + a, Y + b, fc);
        LCD_DrawPoint(X + a, Y - b, fc);
        LCD_DrawPoint(X - a, Y - b, fc);
        LCD_DrawPoint(X + b, Y + a, fc);
        LCD_DrawPoint(X - b, Y + a, fc);
        LCD_DrawPoint(X + b, Y - a, fc);
        LCD_DrawPoint(X - b, Y - a, fc);
    }

    LCD_CS_SET;
}

/**
  * @brief  Draw line in LCD.
  * @param  X0, Y0, X1, Y1: position of the line
  * @param  Color: color of the line
  * @retval None
  */
void LCD_DrawLine(uint16_t X0, 
                  uint16_t Y0, 
                  uint16_t X1, 
                  uint16_t Y1, 
                  uint16_t Color)
{
    /* -difference in x's
       -difference in y's
       -dx,dy * 2
       -amount in pixel space to move during drawing
       -amount in pixel space to move during drawing
       -the discriminant i.e. error i.e. decision variable
       -used for looping
    */
    int dX, dY, dX2, dY2, X_Inc, Y_Inc, Error, Index;

    LCD_CS_CLR;

    LCD_SetXY(X0, Y0);

    /* Calculate X distance */
    dX = X1 - X0;

    /* Calculate Y distance */
    dY = Y1 - Y0;

    if (dX >= 0)
    {
        X_Inc = 1;
    }
    else
    {
        X_Inc = -1;
        dX = -dX;
    } 

    if (dY >= 0)
    {
        Y_Inc = 1;
    }
    else
    {
        Y_Inc = -1;
        dY = -dY; 
    } 

    dX2 = dX << 1;
    dY2 = dY << 1;

    if (dX > dY)
    {
        /* initialize error */
        Error = dY2 - dX;

        /* draw the line */
        for (Index = 0; Index <= dX; Index++)
        {
            LCD_DrawPoint(X0, Y0, Color);

            /* test if error has overflowed */
            if (Error >= 0)
            {
                Error -= dX2;

                /* move to next line */
                Y0 += Y_Inc;
             }

             /* adjust the error term */
             Error += dY2;

             /* move to the next pixel */
             X0 += X_Inc;
        }
    }
    else
    {
        /* initialize error term */
        Error = dX2 - dY;

        /* draw the linedraw the line*/
        for (Index = 0; Index <= dY; Index++)
        {
            /* set the pixel */
            LCD_DrawPoint(X0, Y0, Color);

            /* test if error overflowed */
            if (Error >= 0)
            {
                Error -= dY2;

                /* move to next line */
                X0 += X_Inc;
            }

            /* adjust the error term */
            Error += dX2;

            /* move to the next pixel */
            Y0 += Y_Inc;
        }
    }

    LCD_CS_SET;
}

/**
  * @brief  LCD Box.
  * @param  X,Y: start position of the Box
  * @param  W: Box width
  * @param  H: Box height
  * @param  bc: back color
  * @retval None
  */
void LCD_Box (uint16_t X, 
              uint16_t Y, 
              uint16_t W, 
              uint16_t H, 
              uint16_t bc)
{
    LCD_CS_CLR;

    LCD_DrawLine(X, Y, X + W, Y, GRAY0);
    LCD_DrawLine(X + W - 1, Y + 1, X + W - 1, Y + 1 + H, 0x2965);
    LCD_DrawLine(X, Y + H, X + W, Y + H, 0x2965);
    LCD_DrawLine(X, Y, X, Y + H, GRAY0);
    LCD_DrawLine(X + 1, Y + 1, X + 1 + W - 2, Y + 1 + H - 2, bc);

    LCD_CS_SET;
}

/**
  * @brief  LCD Box2.
  * @param  X,Y: start position of the Box
  * @param  W: Box width
  * @param  H: Box height
  * @param  mode: box mode
  * @retval None
  */
void LCD_Box2 (uint16_t X, 
               uint16_t Y, 
               uint16_t W, 
               uint16_t H, 
               uint8_t  mode)
{
    LCD_CS_CLR;

    if(mode == 0)
    {
        LCD_DrawLine(X, Y, X + W, Y, GRAY0);
        LCD_DrawLine(X + W - 1, Y + 1, X + W - 1, Y + 1 + H, 0x2965);
        LCD_DrawLine(X, Y + H, X + W, Y + H, 0x2965);
        LCD_DrawLine(X, Y, X, Y + H, GRAY0);
    }
    else if(mode == 1)
    {
        LCD_DrawLine(X, Y, X + W, Y, 0x2965);
        LCD_DrawLine(X + W - 1, Y + 1, X + W - 1, Y + 1 + H, GRAY0);
        LCD_DrawLine(X, Y + H, X + W, Y + H, GRAY0);
        LCD_DrawLine(X, Y, X, Y + H, 0x2965);
    }
    else if(mode==2)
    {
        LCD_DrawLine(X, Y, X + W, Y, 0xffff);
        LCD_DrawLine(X + W - 1, Y + 1, X + W - 1, Y + 1 + H, 0xffff);
        LCD_DrawLine(X, Y + H, X + W, Y + H, 0xffff);
        LCD_DrawLine(X, Y, X, Y + H, 0xffff);
    }

    LCD_CS_SET;
}


/**
  * @brief Set a rectangle according to the specified position and color.
  * @param  X1, Y1, X2, Y2: position of the rectangle
  * @param  fc: rectangle color
  * @retval None
  */
void LCD_Rect (uint16_t X1, 
               uint16_t Y1, 
               uint16_t X2, 
               uint16_t Y2, 
               uint16_t fc)
{
    int IndexX,IndexY;

    LCD_CS_CLR;

    for(IndexX = X1; IndexX < X2; IndexX++)
    {
        for(IndexY = Y1; IndexY < Y2; IndexY++)
        {
            /* set the pixel */
            LCD_DrawPoint(IndexX, IndexY, fc);
        }
    }

    LCD_CS_SET;
}

/**
  * @brief  Display button down.
  * @param  x1,y1,x2,y2.
  * @retval None
  */
void DisplayButtonDown(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    LCD_CS_CLR;

    LCD_DrawLine(x1, y1, x2, y1, GRAY2);
    LCD_DrawLine(x1+1, y1+1, x2, y1+1, GRAY1);
    LCD_DrawLine(x1, y1, x1, y2, GRAY2);
    LCD_DrawLine(x1+1, y1+1, x1+1, y2, GRAY1);
    LCD_DrawLine(x1, y2, x2, y2, WHITE);
    LCD_DrawLine(x2, y1, x2, y2, WHITE);

    LCD_CS_SET;
}

/**
  * @brief  Display button up.
  * @param  x1,y1,x2,y2.
  * @retval None
  */
void DisplayButtonUp(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    LCD_CS_CLR;

    LCD_DrawLine(x1, y1, x2, y1, WHITE);
    LCD_DrawLine(x1, y1, x1, y2, WHITE);
    LCD_DrawLine(x1 + 1, y2 - 1, x2, y2 - 1, GRAY1);
    LCD_DrawLine(x1, y2, x2, y2, GRAY2);
    LCD_DrawLine(x2 - 1, y1 + 1, x2 - 1, y2, GRAY1);
    LCD_DrawLine(x2, y1, x2, y2, GRAY2);

    LCD_CS_SET;
}

/**
  * @brief  LCD draw font to GBK16.
  * @param  X, Y: start position of the font
  * @param  fc: forth color
  * @param  bc: back color
  * @param  s: pointer to string
  * @retval None
  */
void LCD_DrawFont_GBK16 (uint16_t X, 
                         uint16_t Y, 
                         uint16_t fc, 
                         uint16_t bc, 
                         uint8_t *s)
{
    unsigned char i, j;
    unsigned short k, x0;

    x0 = X;

    LCD_CS_CLR;

    while(*s)
    {
        /* ASCII character table from 32 to 128 */
        if((*s) < 128)
        {
            k = *s;
            if(k == 13) 
            {
                X = x0;
                Y += 16;
            }
            else 
            {
                if (k > 32)
                {
                    k -= 32;
                }
                else
                {
                    k = 0;
                }

                for(i = 0; i < 16; i++)
                {
                    for(j = 0; j < 8; j++) 
                    {
                        if(asc16[k * 16 + i] & (0x80 >> j))
                        {
                            LCD_DrawPoint(X + j, Y + i, fc);
                        }
                        else 
                        {
                            if (fc != bc) LCD_DrawPoint(X + j, Y + i, bc);
                        }
                    }
                }

                X += 8;
            }

            s++;
        }

        else
        {
            for (k = 0; k < hz16_num; k++) 
            {
                if ((hz16[k].Index[0] == *(s)) && (hz16[k].Index[1] == *(s + 1)))
                { 
                    for(i = 0; i < 16; i++)
                    {
                        for(j = 0; j < 8; j++) 
                        {
                            if(hz16[k].Msk[i * 2] & (0x80 >> j))
                            {
                                LCD_DrawPoint(X + j, Y + i, fc);
                            }
                            else
                            {
                                if(fc != bc) LCD_DrawPoint(X + j, Y + i, bc);
                            }
                        }

                        for(j = 0; j < 8; j++) 
                        {
                            if(hz16[k].Msk[i * 2 + 1] & (0x80 >> j))
                            {
                                LCD_DrawPoint(X + j + 8, Y + i, fc);
                            }
                            else
                            {
                                if(fc != bc) LCD_DrawPoint(X + j + 8, Y + i, bc);
                            }
                        }
                    }
                }
            }

            s += 2;
            X += 16;
        }
    }

    LCD_CS_SET;
}

/**
  * @brief  LCD draw font to GBK24.
  * @param  X, Y: start position of the font
  * @param  fc: forth color
  * @param  bc: back color
  * @param  s: pointer to string
  * @retval None
  */
void LCD_DrawFont_GBK24 (uint16_t X, 
                         uint16_t Y, 
                         uint16_t fc, 
                         uint16_t bc, 
                         uint8_t *s)
{
    unsigned char i, j;
    unsigned short k;

    LCD_CS_CLR;

    while(*s) 
    {
        if(*s < 0x80)
        {
            k = *s;

            if(k > 32)
            {
                k -= 32;
            }
            else
            {
                k = 0;
            }

            for(i = 0; i < 16; i++)
            {
                for(j = 0; j < 8; j++) 
                {
                    if(asc16[k * 16 + i] & (0x80 >> j))
                    {
                        LCD_DrawPoint(X + j, Y + i, fc);
                    }
                    else 
                    {
                        if(fc != bc) LCD_DrawPoint(X + j, Y + i, bc);
                    }
                }
            }

            s++;
            X += 8;
        }
        else 
        {
            for(k = 0; k < hz24_num; k++) 
            {
                if ((hz24[k].Index[0] == *(s)) && (hz24[k].Index[1] == *(s+1)))
                { 
                    for(i = 0; i < 24; i++)
                    {
                        for(j = 0; j < 8; j++) 
                        {
                            if(hz24[k].Msk[i * 3] & (0x80 >> j))
                            {
                                LCD_DrawPoint(X + j, Y + i, fc);
                            }
                            else 
                            {
                                if(fc != bc) LCD_DrawPoint(X + j, Y + i, bc);
                            }
                        }

                        for(j = 0; j < 8; j++) 
                        {
                            if(hz24[k].Msk[i * 3 + 1] & (0x80 >> j))
                            {
                                LCD_DrawPoint(X + j + 8, Y + i, fc);
                            }
                            else
                            {
                                if(fc != bc) LCD_DrawPoint(X + j + 8, Y + i, bc);
                            }
                        }

                        for(j = 0; j < 8; j++) 
                        {
                            if(hz24[k].Msk[i * 3 + 2] & (0x80 >> j))
                            {
                                LCD_DrawPoint(X + j + 16, Y + i, fc);
                            }
                            else 
                            {
                                if(fc != bc) LCD_DrawPoint(X + j + 16, Y + i, bc);
                            }
                        }
                    }
                }
            }

            s += 2;
            X += 24;
        }
    }

    LCD_CS_SET;
}

/**
  * @brief  LCD draw font to num32.
  * @param  X, Y: start position of the font
  * @param  fc: forth color
  * @param  bc: back color
  * @param  num: number
  * @retval None
  */
void Gui_DrawFont_Num32 (uint16_t X, 
                         uint16_t Y, 
                         uint16_t fc, 
                         uint16_t bc, 
                         uint16_t num)
{
    unsigned char i, j, k, c;

    LCD_CS_CLR;

    for(i = 0; i < 32; i++)
    {
        for(j = 0; j < 4; j++)
        {
            c = *(sz32 + num * 32 * 4 + i * 4 + j);

            for (k = 0; k < 8; k++)
            {
                if(c & (0x80 >> k))
                {
                    LCD_DrawPoint(X + j * 8 + k, Y + i, fc);
                }
                else
                {
                    if(fc != bc) LCD_DrawPoint(X + j * 8 + k, Y + i, bc);
                }
            }
        }
    }

    LCD_CS_SET;
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
  
/**
  * @}
  */
  
/**
  * @}
  */

#endif

/************************ (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
