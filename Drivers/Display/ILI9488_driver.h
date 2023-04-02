/* 
 * File:   ILI9488_driver.h
 * Author: Mattia
 *
 * Created on December 14, 2021, 7:48 AM
 */

#ifndef ILI9488_DRIVER_H
#define	ILI9488_DRIVER_H

#ifdef __XC32
    #include <xc.h>          /* Defines special funciton registers, CP0 regs  */
#endif
#define _SUPPRESS_PLIB_WARNING
#define _DISABLE_OPENADC10_CONFIGPORT_WARNING
#include <plib.h>            /* Include to use PIC32 peripheral libraries     */
#include <stdint.h>          /* For uint32_t definition                       */
#include <stdbool.h>         /* For true/false definition                     */
#include <string.h>
#include "../../user.h"

#define LCD_RES         LATEbits.LATE5
#define LCD_DC          LATEbits.LATE6
#define LCD_CS          LATEbits.LATE7

#define LCD_RES_DIR     TRISEbits.TRISE5
#define LCD_DC_DIR      TRISEbits.TRISE6
#define LCD_CS_DIR      TRISEbits.TRISE7

#define ILI9488_TFTWIDTH  320
#define ILI9488_TFTHEIGHT 480

#define ILI9488_NOP     0x00
#define ILI9488_SWRESET 0x01
#define ILI9488_RDDID   0x04
#define ILI9488_RDDST   0x09

#define ILI9488_SLPIN   0x10
#define ILI9488_SLPOUT  0x11
#define ILI9488_PTLON   0x12
#define ILI9488_NORON   0x13

#define ILI9488_RDMODE  0x0A
#define ILI9488_RDMADCTL  0x0B
#define ILI9488_RDPIXFMT  0x0C
#define ILI9488_RDIMGFMT  0x0D
#define ILI9488_RDSELFDIAG  0x0F

#define ILI9488_INVOFF  0x20
#define ILI9488_INVON   0x21
#define ILI9488_GAMMASET 0x26
#define ILI9488_DISPOFF 0x28
#define ILI9488_DISPON  0x29

#define ILI9488_CASET   0x2A
#define ILI9488_RASET   0x2B
#define ILI9488_RAMWR   0x2C
#define ILI9488_RAMRD   0x2E

#define ILI9488_PTLAR   0x30
#define ILI9488_MADCTL  0x36
#define ILI9488_PIXFMT  0x3A

#define ILI9488_FRMCTR1 0xB1
#define ILI9488_FRMCTR2 0xB2
#define ILI9488_FRMCTR3 0xB3
#define ILI9488_INVCTR  0xB4
#define ILI9488_DFUNCTR 0xB6

#define ILI9488_PWCTR1  0xC0
#define ILI9488_PWCTR2  0xC1
#define ILI9488_PWCTR3  0xC2
#define ILI9488_PWCTR4  0xC3
#define ILI9488_PWCTR5  0xC4
#define ILI9488_VMCTR1  0xC5
#define ILI9488_VMCTR2  0xC7

#define ILI9488_RDID1   0xDA
#define ILI9488_RDID2   0xDB
#define ILI9488_RDID3   0xDC
#define ILI9488_RDID4   0xDD

#define ILI9488_GMCTRP1 0xE0
#define ILI9488_GMCTRN1 0xE1
/*
#define ILI9488_PWCTR6  0xFC
*/

// Color definitions
#define ILI9488_BLACK       0x0000      /*   0,   0,   0 */
#define ILI9488_NAVY        0x000F      /*   0,   0, 128 */
#define ILI9488_DARKGREEN   0x03E0      /*   0, 128,   0 */
#define ILI9488_DARKCYAN    0x03EF      /*   0, 128, 128 */
#define ILI9488_MAROON      0x7800      /* 128,   0,   0 */
#define ILI9488_PURPLE      0x780F      /* 128,   0, 128 */
#define ILI9488_OLIVE       0x7BE0      /* 128, 128,   0 */
#define ILI9488_LIGHTGREY   0xC618      /* 192, 192, 192 */
#define ILI9488_DARKGREY    0x7BEF      /* 128, 128, 128 */
#define ILI9488_BLUE        0x001F      /*   0,   0, 255 */
#define ILI9488_GREEN       0x07E0      /*   0, 255,   0 */
#define ILI9488_CYAN        0x07FF      /*   0, 255, 255 */
#define ILI9488_RED         0xF800      /* 255,   0,   0 */
#define ILI9488_MAGENTA     0xF81F      /* 255,   0, 255 */
#define ILI9488_YELLOW      0xFFE0      /* 255, 255,   0 */
#define ILI9488_WHITE       0xFFFF      /* 255, 255, 255 */
#define ILI9488_ORANGE      0xFD20      /* 255, 165,   0 */
#define ILI9488_GREENYELLOW 0xAFE5      /* 173, 255,  47 */
#define ILI9488_PINK        0xF81F

#define MADCTL_MY  0x80
#define MADCTL_MX  0x40
#define MADCTL_MV  0x20
#define MADCTL_ML  0x10
#define MADCTL_RGB 0x00
#define MADCTL_BGR 0x08
#define MADCTL_MH  0x04

//These enumerate the text plotting alignment (reference datum point)
#define TL_DATUM 0 // Top left (default)
#define TC_DATUM 1 // Top centre
#define TR_DATUM 2 // Top right
#define ML_DATUM 3 // Middle left
#define CL_DATUM 3 // Centre left, same as above
#define MC_DATUM 4 // Middle centre
#define CC_DATUM 4 // Centre centre, same as above
#define MR_DATUM 5 // Middle right
#define CR_DATUM 5 // Centre right, same as above
#define BL_DATUM 6 // Bottom left
#define BC_DATUM 7 // Bottom centre
#define BR_DATUM 8 // Bottom right
#define L_BASELINE  9 // Left character baseline (Line the 'A' character would sit on)
#define C_BASELINE 10 // Centre character baseline
#define R_BASELINE 11 // Right character baseline

typedef struct
{
    int16_t  _x1, _y1; // Coordinates of top-left corner of button
    int16_t  _xd, _yd; // Button text datum offsets (wrt centre of button)
    uint16_t _w, _h;   // Width and height of button
    uint8_t  _textsize, _textdatum; // Text size multiplier and text datum for button
    uint16_t _outlinecolor, _fillcolor, _textcolor;
    char     _label[10]; // Button text is 9 chars maximum unless long_name used
    bool  currstate, laststate; // Button states
}Button;

void GLCD_init(void);
void setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
static void pushColor(uint16_t color);
void pushColors(uint16_t color, uint32_t len);
void drawImage(const uint8_t* img, uint16_t x, uint16_t y, uint16_t w, uint16_t h);
void fillScreen(uint16_t color);
void drawPixel(uint16_t x, uint16_t y, uint16_t color);
void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
void drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
void fillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
void drawRoundRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t r, uint16_t color);
void fillRoundRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t r, uint16_t color);
void setRotation(uint8_t r);
void invertDisplay(BOOL i);
void drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color,
              uint16_t bg, uint8_t size_x, uint8_t size_y);
uint16_t drawString(char *string, uint16_t poX, uint16_t poY);
void writeString(char *s, uint16_t xPos, uint16_t yPos);
void setTextSize(uint8_t s);
void setCursor(uint16_t x, uint16_t y);
//void setTextColor(uint16_t c, uint16_t bg);
void setTextWrap(bool w);
void setTextColor(uint16_t color, uint16_t bg);

uint16_t getTextColor();
uint16_t getTextBgColor();
uint16_t getWidth(void);
uint16_t getHeight(void);
uint8_t getRotation(void);
int16_t getCursorX(void);
int16_t getCursorY(void);

void     setTextDatum(uint8_t datum);                     // Set text datum position (default is top left), see Section 6 above 
uint8_t  getTextDatum(void);
void     setTextPadding(uint16_t x_width);                // Set text padding (background blanking/over-write) width in pixels
uint16_t getTextPadding(void);

int16_t textWidth(char *string);

void     setViewport(int32_t x, int32_t y, int32_t w, int32_t h, bool vpDatum);
bool     checkViewport(int32_t x, int32_t y, int32_t w, int32_t h);
int32_t  getViewportX(void);
int32_t  getViewportY(void);
int32_t  getViewportWidth(void);
int32_t  getViewportHeight(void);
bool     getViewportDatum(void);
void     frameViewport(uint16_t color, int32_t w);
void     resetViewport(void);

void initButton(Button *b, int16_t x, int16_t y, uint16_t w, uint16_t h,
 uint16_t outline, uint16_t fill, uint16_t textcolor,
 char *label, uint8_t textsize);
void initButtonUL(Button *b, int16_t x1, int16_t y1, uint16_t w, uint16_t h,
 uint16_t outline, uint16_t fill, uint16_t textcolor,
 char *label, uint8_t textsize);
void setLabelDatum(Button *b, int16_t x_delta, int16_t y_delta, uint8_t datum);
void drawButton(Button *b, bool inverted);
bool contains(Button *b, int16_t x, int16_t y);
void press(Button *b, bool p);
bool isPressed(Button *b);  
bool justPressed(Button *b);
bool justReleased(Button *b);
static void SPI2Setup(uint8_t speed);
static void SPI2Transmit(uint8_t d);
static void writeCommand(uint8_t cmd);
static void writeData(uint8_t data);
#endif	/* ILI9488_DRIVER_H */