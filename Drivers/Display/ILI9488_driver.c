#include "ILI9488_driver.h"
#include "font.c"

uint16_t height = 320;
uint16_t width = 480;
uint8_t rotation = 0;
uint16_t cursor_x = 0;
uint16_t cursor_y = 0;
uint16_t padX;
uint16_t textColor = ILI9488_BLACK;
uint16_t textBgColor = ILI9488_WHITE;
uint8_t textSize = 1;
bool wrap = false;
uint8_t textdatum;  

// Viewport variables
int32_t  _vpX, _vpY, _vpW, _vpH;    // Note: x start, y start, x end + 1, y end + 1
int32_t  _xDatum;
int32_t  _yDatum;
int32_t  _xWidth;
int32_t  _yHeight;
bool     _vpDatum = false;
bool     _vpOoB = false;


void initButton(Button *b, int16_t x, int16_t y, uint16_t w, uint16_t h,
 uint16_t outline, uint16_t fill, uint16_t textcolor,
 char *label, uint8_t textsize)
{
  // Tweak arguments and pass to the newer initButtonUL() function...
  initButtonUL(b, x - (w / 2), y - (h / 2), w, h, outline, fill,
    textcolor, label, textsize);
}
void initButtonUL(Button *b, int16_t x1, int16_t y1, uint16_t w, uint16_t h,
 uint16_t outline, uint16_t fill, uint16_t textcolor,
 char *label, uint8_t textsize)
{
  b->_x1           = x1;
  b->_y1           = y1;
  b->_w            = w;
  b->_h            = h;
  b->_outlinecolor = outline;
  b->_fillcolor    = fill;
  b->_textcolor    = textcolor;
  b->_textsize     = textsize;
  strncpy(b->_label, label, 9);
}

void drawButton(Button *b, bool inverted) {
    uint16_t fill, outline, text;

    if(!inverted) 
    {
        fill    = b->_fillcolor;
        outline = b->_outlinecolor;
        text    = b->_textcolor;
    } else {
        fill    = b->_textcolor;
        outline = b->_outlinecolor;
        text    = b->_fillcolor;
    }

    uint8_t r = 5;//min(_w, _h) / 4; // Corner radius
    fillRoundRect(b->_x1, b->_y1, b->_w, b->_h, r, fill);
    drawRoundRect(b->_x1, b->_y1, b->_w, b->_h, r, outline);

    setTextColor(text, fill);
    setTextSize(b->_textsize);

    uint8_t tempdatum = getTextDatum();
    setTextDatum(b->_textdatum);
    uint16_t tempPadding = getTextPadding();
    setTextPadding(0);

    drawString(b->_label, b->_x1 + (b->_w/2) + b->_xd, b->_y1 + (b->_h/2) - 4 + b->_yd);

    setTextDatum(tempdatum);
    setTextPadding(tempPadding);
}

void setLabelDatum(Button *b, int16_t x_delta, int16_t y_delta, uint8_t datum)
{
  b->_xd        = x_delta;
  b->_yd        = y_delta;
  b->_textdatum = datum;
}

bool contains(Button *b, int16_t x, int16_t y) {
  return ((x >= b->_x1) && (x < (b->_x1 + b->_w)) &&
          (y >= b->_y1) && (y < (b->_y1 + b->_h)));
}
void press(Button *b, bool p) {
  b->laststate = b->currstate;
  b->currstate = p;
}
bool isPressed(Button *b)    { return b->currstate; }
bool justPressed(Button *b)  { return ((b->currstate) && !(b->laststate)); }
bool justReleased(Button *b) { return (!(b->currstate) && (b->laststate)); }

void swap_coord(uint16_t *a, uint16_t *b)
{
    uint16_t t = *a;
    *a = *b;
    *b = t;
}
static void SPI2Setup(uint8_t speed)
{
    SpiChnClose(2);
    SpiChnOpen(2,SPI_OPEN_MSTEN | SPI_OPEN_CKE_REV | SPI_OPEN_MODE8, 8 );
    if(speed)
        SpiChnSetBrg(2, 1); // SPIBRG = 1 for 20MHz @ 80MHz Clock
    else
        SpiChnSetBrg(2, 3);// SPIBRG = 3 for 10MHz @ 80MHz Clock
}
 static void SPI2Transmit(uint8_t d)
{   uint8_t junk;
    SpiChnPutC(2, d);
    junk = SpiChnGetC(2);
}
static void writeCommand(uint8_t cmd) 
{
    LCD_DC = 0;
    SPI2Transmit(0x0);
    SPI2Transmit(cmd);
    
}
static void writeData(uint8_t data)
{
    LCD_DC = 1;
    SPI2Transmit(0x0);
    SPI2Transmit(data);
}
void GLCD_init(void)
{
    LCD_DC_DIR = 0;
    LCD_CS_DIR = 0;
    LCD_RES_DIR = 0;
    LCD_CS = 1;
    SPI2Setup(1);
    
    LCD_RES = 1;
    delay_ms(500);
    LCD_RES = 0;
    delay_ms(500);
    LCD_RES = 1;
    delay_ms(150);
    LCD_CS = 0;
    //ILI9486
    writeCommand(0x11); // Sleep out, also SW reset
    delay_ms(120);

    writeCommand(0x3A);
    writeData(0x55); // 16 bit colour interface
    writeCommand(0xC2);
    writeData(0x44);

    writeCommand(0xC5);
    writeData(0x00);
    writeData(0x00);
    writeData(0x00);
    writeData(0x00);

    writeCommand(0xE0);
    writeData(0x0F);
    writeData(0x1F);
    writeData(0x1C);
    writeData(0x0C);
    writeData(0x0F);
    writeData(0x08);
    writeData(0x48);
    writeData(0x98);
    writeData(0x37);
    writeData(0x0A);
    writeData(0x13);
    writeData(0x04);
    writeData(0x11);
    writeData(0x0D);
    writeData(0x00);

    writeCommand(0xE1);
    writeData(0x0F);
    writeData(0x32);
    writeData(0x2E);
    writeData(0x0B);
    writeData(0x0D);
    writeData(0x05);
    writeData(0x47);
    writeData(0x75);
    writeData(0x37);
    writeData(0x06);
    writeData(0x10);
    writeData(0x03);
    writeData(0x24);
    writeData(0x20);
    writeData(0x00);
    writeCommand(ILI9488_INVON); //ILI9488 display inverted color

    writeCommand(0x36);
    writeData(0x48);

    writeCommand(0x29); // display on
    setRotation(3);
    
    delay_ms(150);
    
    LCD_CS = 1;
}

void setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    uint8_t lowByte, highByte;
    
    LCD_CS = 0;
    writeCommand(ILI9488_CASET);
    LCD_DC = 1;
    lowByte = (uint8_t) (x0 & 0x00FF);
    highByte = (uint8_t) ((x0 & 0xFF00) >> 8);
    SPI2Transmit(lowByte);
    SPI2Transmit(highByte);
    SPI2Transmit(highByte);
    SPI2Transmit(lowByte);
    lowByte = (uint8_t) (x1 & 0x00FF);
    highByte = (uint8_t) ((x1 & 0xFF00) >> 8);
    SPI2Transmit(lowByte);
    SPI2Transmit(highByte);
    SPI2Transmit(highByte);
    SPI2Transmit(lowByte);
    
    writeCommand(ILI9488_RASET);
    LCD_DC = 1;
    lowByte = (uint8_t) (y0 & 0x00FF);
    highByte = (uint8_t) ((y0 & 0xFF00) >> 8);
    SPI2Transmit(lowByte);
    SPI2Transmit(highByte);
    SPI2Transmit(highByte);
    SPI2Transmit(lowByte); 
    lowByte = (uint8_t) (y1 & 0x00FF);
    highByte = (uint8_t) ((y1 & 0xFF00) >> 8);
    SPI2Transmit(lowByte);
    SPI2Transmit(highByte);
    SPI2Transmit(highByte);
    SPI2Transmit(lowByte);
    
    LCD_CS = 1;
}

static void pushColor(uint16_t color)
{
    SPI2Transmit((uint8_t) ((color  & 0xFF00 ) >> 8));
    SPI2Transmit((uint8_t) color & 0x00FF);
}

void pushColors(uint16_t color, uint32_t len) 
{
    LCD_CS = 0;
    writeCommand(ILI9488_RAMWR);
    LCD_DC = 1;
    while(len--)
    {
        pushColor(color);
    }
    LCD_CS = 1;
    
}

void drawImage(const uint8_t* img, uint16_t x, uint16_t y, uint16_t w, uint16_t h) 
{
    uint16_t i;
    uint16_t o;
    // rudimentary clipping (drawChar w/big text requires this)
    if ((x >= width) || (y >= height)) 
        return;
    if ((x + w - 1) >= width) 
        w = width - x;
    if ((y + h - 1) >= height) 
        h = height - y;

    setAddrWindow(x, y, x + w - 1, y + h - 1);
    LCD_DC = 1;
    LCD_CS = 0;

    uint8_t lineBuff[w * 3 + 1];
    uint32_t count = 0;
    
    for (i = 0; i < h; i++) {
        uint16_t pixCount = 0;
        for (o = 0; o < w; o++) 
        {
            uint8_t b1 = img[count];
            count++;
            uint8_t b2 = img[count];
            count++;
            uint16_t color = b1 << 8 | b2;
            lineBuff[pixCount] = (((color & 0xF800) >> 11)* 255) / 31;
            pixCount++;
            lineBuff[pixCount] = (((color & 0x07E0) >> 5) * 255) / 63;
            pixCount++;
            lineBuff[pixCount] = ((color & 0x001F)* 255) / 31;
            pixCount++;
        } // for row

        for (i = 0; i < w * 3; i++) 
            SPI2Transmit(lineBuff[i]);
    }// for col
    LCD_CS = 1;
}

void drawPixel(uint16_t x, uint16_t y, uint16_t color) 
{
    if ((x < 0) || (x > width) || (y < 0) || (y > height)) 
        return;
    LCD_CS = 0;
    setAddrWindow(x, y, x, y );
    pushColors(color, 1);
    LCD_CS = 1;
}
void drawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
{
  drawFastHLine(x, y, w, color);
  drawFastHLine(x, y + h - 1, w, color);
  // Avoid drawing corner pixels twice
  drawFastVLine(x, y+1, h-2, color);
  drawFastVLine(x + w - 1, y+1, h-2, color);
}

void drawCircleHelper( uint16_t x0, uint16_t y0, uint16_t rr, uint8_t cornername, uint16_t color)
{
  if (rr <= 0) return;
  int32_t f     = 1 - rr;
  int32_t ddF_x = 1;
  int32_t ddF_y = -2 * rr;
  int32_t xe    = 0;
  int32_t xs    = 0;
  int32_t len   = 0;

  while (xe < rr--)
  {
    while (f < 0) 
    {
      ++xe;
      f += (ddF_x += 2);
    }
    f += (ddF_y += 2);

    if (xe-xs==1) 
    {
      if (cornername & 0x1) { // left top
        drawPixel(x0 - xe, y0 - rr, color);
        drawPixel(x0 - rr, y0 - xe, color);
      }
      if (cornername & 0x2) { // right top
        drawPixel(x0 + rr    , y0 - xe, color);
        drawPixel(x0 + xs + 1, y0 - rr, color);
      }
      if (cornername & 0x4) { // right bottom
        drawPixel(x0 + xs + 1, y0 + rr    , color);
        drawPixel(x0 + rr, y0 + xs + 1, color);
      }
      if (cornername & 0x8) { // left bottom
        drawPixel(x0 - rr, y0 + xs + 1, color);
        drawPixel(x0 - xe, y0 + rr    , color);
      }
    }
    else 
    {
      len = xe - xs++;
      if (cornername & 0x1) { // left top
        drawFastHLine(x0 - xe, y0 - rr, len, color);
        drawFastVLine(x0 - rr, y0 - xe, len, color);
      }
      if (cornername & 0x2) { // right top
        drawFastVLine(x0 + rr, y0 - xe, len, color);
        drawFastHLine(x0 + xs, y0 - rr, len, color);
      }
      if (cornername & 0x4) { // right bottom
        drawFastHLine(x0 + xs, y0 + rr, len, color);
        drawFastVLine(x0 + rr, y0 + xs, len, color);
      }
      if (cornername & 0x8) { // left bottom
        drawFastVLine(x0 - rr, y0 + xs, len, color);
        drawFastHLine(x0 - xe, y0 + rr, len, color);
      }
    }
    xs = xe;
  }
}
void drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color)
{  
    if(x0 == x1)
    {
        if(y0 > y1)
            swap_coord(&y0, &y1);
        drawFastVLine(x0,y0, y1 - y0 , color);
    }
    else if(y0 == y1)
        {
            if(x0 > x1)
                swap_coord(&x0, &x1);
            drawFastHLine(x0, y0, x1 - x0 , color);
        }
        else
        {
            int16_t steep = abs(y1 - y0) > abs(x1 - x0);
            if (steep) 
            {
                swap_coord(&x0, &y0);
                swap_coord(&x1, &y1);
            }

            if (x0 > x1) 
            {
                swap_coord(&x0, &x1);
                swap_coord(&y0, &y1);
            }

            int16_t dx, dy;
            dx = x1 - x0;
            dy = abs(y1 - y0);

            int16_t err = dx / 2;
            int16_t ystep;

            if (y0 < y1)
                ystep = 1;
            else
                ystep = -1;

          for (; x0 <= x1; x0++) 
          {
            if (steep) 
                drawPixel(y0, x0, color);
             else
                drawPixel(x0, y0, color);
            err -= dy;
            if (err < 0) 
            {
                y0 += ystep;
                err += dx;
            }
          }    
        }
}

void drawRoundRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t r, uint16_t color)
{

  // smarter version
  drawFastHLine(x + r  , y    , w - r - r, color); // Top
  drawFastHLine(x + r  , y + h - 1, w - r - r, color); // Bottom
  drawFastVLine(x    , y + r  , h - r - r, color); // Left
  drawFastVLine(x + w - 1, y + r  , h - r - r, color); // Right
  // draw four corners
  drawCircleHelper(x + r    , y + r    , r, 1, color);
  drawCircleHelper(x + w - r - 1, y + r    , r, 2, color);
  drawCircleHelper(x + w - r - 1, y + h - r - 1, r, 4, color);
  drawCircleHelper(x + r    , y + h - r - 1, r, 8, color);
}
void drawCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color)
{
  if ( r <= 0 ) return;

    int32_t f     = 1 - r;
    int32_t ddF_y = -2 * r;
    int32_t ddF_x = 1;
    int32_t xs    = -1;
    int32_t xe    = 0;
    int32_t len   = 0;
    
    bool first = true;
    do {
      while (f < 0) 
      {
        ++xe;
        f += (ddF_x += 2);
      }
      f += (ddF_y += 2);

      if (xe-xs>1) 
      {
        if (first) 
        {
          len = 2*(xe - xs)-1;
          drawFastHLine(x0 - xe, y0 + r, len, color);
          drawFastHLine(x0 - xe, y0 - r, len, color);
          drawFastVLine(x0 + r, y0 - xe, len, color);
          drawFastVLine(x0 - r, y0 - xe, len, color);
          first = false;
        }
        else 
        {
          len = xe - xs++;
          drawFastHLine(x0 - xe, y0 + r, len, color);
          drawFastHLine(x0 - xe, y0 - r, len, color);
          drawFastHLine(x0 + xs, y0 - r, len, color);
          drawFastHLine(x0 + xs, y0 + r, len, color);

          drawFastVLine(x0 + r, y0 + xs, len, color);
          drawFastVLine(x0 + r, y0 - xe, len, color);
          drawFastVLine(x0 - r, y0 - xe, len, color);
          drawFastVLine(x0 - r, y0 + xs, len, color);
        }
      }
      else 
      {
        ++xs;
        drawPixel(x0 - xe, y0 + r, color);
        drawPixel(x0 - xe, y0 - r, color);
        drawPixel(x0 + xs, y0 - r, color);
        drawPixel(x0 + xs, y0 + r, color);

        drawPixel(x0 + r, y0 + xs, color);
        drawPixel(x0 + r, y0 - xe, color);
        drawPixel(x0 - r, y0 - xe, color);
        drawPixel(x0 - r, y0 + xs, color);
      }
      xs = xe;
    } while (xe < --r);
    
}

void drawEllipse(uint16_t x0, uint16_t y0, uint16_t rx, uint16_t ry, uint16_t color)
{
  if (rx<2) return;
  if (ry<2) return;
  int32_t x, y;
  int32_t rx2 = rx * rx;
  int32_t ry2 = ry * ry;
  int32_t fx2 = 4 * rx2;
  int32_t fy2 = 4 * ry2;
  int32_t s;

  for (x = 0, y = ry, s = 2*ry2+rx2*(1-2*ry); ry2*x <= rx2*y; x++) 
  {
    // These are ordered to minimise coordinate changes in x or y
    // drawPixel can then send fewer bounding box commands
    drawPixel(x0 + x, y0 + y, color);
    drawPixel(x0 - x, y0 + y, color);
    drawPixel(x0 - x, y0 - y, color);
    drawPixel(x0 + x, y0 - y, color);
    if (s >= 0) 
    {
      s += fx2 * (1 - y);
      y--;
    }
    s += ry2 * ((4 * x) + 6);
  }

  for (x = rx, y = 0, s = 2*rx2+ry2*(1-2*rx); rx2*y <= ry2*x; y++) 
  {
    // These are ordered to minimise coordinate changes in x or y
    // drawPixel can then send fewer bounding box commands
    drawPixel(x0 + x, y0 + y, color);
    drawPixel(x0 - x, y0 + y, color);
    drawPixel(x0 - x, y0 - y, color);
    drawPixel(x0 + x, y0 - y, color);
    if (s >= 0)
    {
      s += fy2 * (1 - x);
      x--;
    }
    s += rx2 * ((4 * y) + 6);
  }
}

void fillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) 
{
    // rudimentary clipping (drawChar w/big text requires this)
//    if ((x >= width) || (y >= height)) 
//        return;
//    if ((x + w - 1) >= width) 
//        w = width - x;
//    if ((y + h - 1) >= height) 
//        h = height - y;

    setAddrWindow(x, y, x + w - 1, y + h - 1);
    LCD_DC = 1;
    LCD_CS = 0;
    pushColors(color, w*h);
    LCD_CS = 1;
}

void fillCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color)
{
  int32_t  x  = 0;
  int32_t  dx = 1;
  int32_t  dy = r+r;
  int32_t  p  = -(r>>1);
  drawFastHLine(x0 - r, y0, dy+1, color);

  while(x<r)
  {

    if(p>=0) 
    {
      drawFastHLine(x0 - x, y0 + r, dx, color);
      drawFastHLine(x0 - x, y0 - r, dx, color);
      dy-=2;
      p-=dy;
      r--;
    }

    dx+=2;
    p+=dx;
    x++;

    drawFastHLine(x0 - r, y0 + x, dy+1, color);
    drawFastHLine(x0 - r, y0 - x, dy+1, color);

  }
}

void fillCircleHelper(uint16_t x0, uint16_t y0, uint16_t r, uint8_t cornername, int32_t delta, uint16_t color)
{
  int32_t f     = 1 - r;
  int32_t ddF_x = 1;
  int32_t ddF_y = -r - r;
  int32_t y     = 0;

  delta++;

  while (y < r) 
  {
    if (f >= 0) 
    {
      if (cornername & 0x1) 
          drawFastHLine(x0 - y, y0 + r, y + y + delta, color);
      if (cornername & 0x2) 
          drawFastHLine(x0 - y, y0 - r, y + y + delta, color);
      r--;
      ddF_y += 2;
      f     += ddF_y;
    }

    y++;
    ddF_x += 2;
    f     += ddF_x;

    if (cornername & 0x1) 
        drawFastHLine(x0 - r, y0 + y, r + r + delta, color);
    if (cornername & 0x2) 
        drawFastHLine(x0 - r, y0 - y, r + r + delta, color);
  }
}

void fillEllipse(int16_t x0, int16_t y0, int32_t rx, int32_t ry, uint16_t color)
{
  if (rx<2) return;
  if (ry<2) return;
  int32_t x, y;
  int32_t rx2 = rx * rx;
  int32_t ry2 = ry * ry;
  int32_t fx2 = 4 * rx2;
  int32_t fy2 = 4 * ry2;
  int32_t s;
  for (x = 0, y = ry, s = 2*ry2+rx2*(1-2*ry); ry2*x <= rx2*y; x++) 
  {
    drawFastHLine(x0 - x, y0 - y, x + x + 1, color);
    drawFastHLine(x0 - x, y0 + y, x + x + 1, color);

    if (s >= 0) 
    {
      s += fx2 * (1 - y);
      y--;
    }
    s += ry2 * ((4 * x) + 6);
  }

  for (x = rx, y = 0, s = 2*rx2+ry2*(1-2*rx); rx2*y <= ry2*x; y++) 
  {
    drawFastHLine(x0 - x, y0 - y, x + x + 1, color);
    drawFastHLine(x0 - x, y0 + y, x + x + 1, color);

    if (s >= 0) 
    {
      s += fy2 * (1 - x);
      x--;
    }
    s += rx2 * ((4 * y) + 6);
  }

}

void fillRoundRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t r, uint16_t color)
{
  // smarter version
  fillRect(x, y + r, w, h - r - r, color);
  // draw four corners
  fillCircleHelper(x + r, y + h - r - 1, r, 1, w - r - r - 1, color);
  fillCircleHelper(x + r    , y + r, r, 2, w - r - r - 1, color);
}

void drawTriangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
  drawLine(x0, y0, x1, y1, color);
  drawLine(x1, y1, x2, y2, color);
  drawLine(x2, y2, x0, y0, color);
}


/**************************************************************************/
/*!
   @brief     Draw a triangle with color-fill
    @param    x0  Vertex #0 x coordinate
    @param    y0  Vertex #0 y coordinate
    @param    x1  Vertex #1 x coordinate
    @param    y1  Vertex #1 y coordinate
    @param    x2  Vertex #2 x coordinate
    @param    y2  Vertex #2 y coordinate
    @param    color 16-bit 5-6-5 Color to fill/draw with
*/
/**************************************************************************/
void fillTriangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,
                                uint16_t x2, uint16_t y2, uint16_t color) {

  int16_t a, b, y, last;

  // Sort coordinates by Y order (y2 >= y1 >= y0)
  if (y0 > y1) {
    swap_coord(&y0, &y1);
    swap_coord(&x0, &x1);
  }
  if (y1 > y2) {
    swap_coord(&y2, &y1);
    swap_coord(&x2, &x1);
  }
  if (y0 > y1) {
    swap_coord(&y0, &y1);
    swap_coord(&x0, &x1);
  }
  if (y0 == y2) { // Handle awkward all-on-same-line case as its own thing
    a = b = x0;
    if (x1 < a)
      a = x1;
    else if (x1 > b)
      b = x1;
    if (x2 < a)
      a = x2;
    else if (x2 > b)
      b = x2;
    drawFastHLine(a, y0, b - a + 1, color);
    return;
  }

  int16_t dx01 = x1 - x0, dy01 = y1 - y0, dx02 = x2 - x0, dy02 = y2 - y0,
          dx12 = x2 - x1, dy12 = y2 - y1;
  int32_t sa = 0, sb = 0;

  // For upper part of triangle, find scanline crossings for segments
  // 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
  // is included here (and second loop will be skipped, avoiding a /0
  // error there), otherwise scanline y1 is skipped here and handled
  // in the second loop...which also avoids a /0 error here if y0=y1
  // (flat-topped triangle).
  if (y1 == y2)
    last = y1; // Include y1 scanline
  else
    last = y1 - 1; // Skip it

  for (y = y0; y <= last; y++) {
    a = x0 + sa / dy01;
    b = x0 + sb / dy02;
    sa += dx01;
    sb += dx02;
    /* longhand:
    a = x0 + (x1 - x0) * (y - y0) / (y1 - y0);
    b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
    */
    if (a > b)
      swap_coord(&a, &b);
    drawFastHLine(a, y, b - a + 1, color);
  }

  // For lower part of triangle, find scanline crossings for segments
  // 0-2 and 1-2.  This loop is skipped if y1=y2.
  sa = (int32_t)dx12 * (y - y1);
  sb = (int32_t)dx02 * (y - y0);
  for (; y <= y2; y++) {
    a = x1 + sa / dy12;
    b = x0 + sb / dy02;
    sa += dx12;
    sb += dx02;
    /* longhand:
    a = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
    b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
    */
    if (a > b)
      swap_coord(&a, &b);
    drawFastHLine(a, y, b - a + 1, color);
  }
}


void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) 
{

    // Rudimentary clipping
    if ((x > width) || (y > height)) 
        return;
    if ((y + h - 1) > height)
        h = height - y;

    setAddrWindow(x, y, x, y + h - 1);
    LCD_DC = 1;
    LCD_CS = 0;
    pushColors(color, h);
    LCD_CS = 1;
}

void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) 
{
    // Rudimentary clipping
    if ((x > width) || (y > height)) 
        return;
    if ((x + w - 1) > width) 
        w = width - x;
    
    setAddrWindow(x, y, x + w - 1, y);
    LCD_DC = 1;
    LCD_CS = 0;
    pushColors(color, w);
    LCD_CS = 1;
}

void fillScreen(uint16_t color)
{
    fillRect(0, 0, width, height, color);
}

void setRotation(uint8_t m) {
    writeCommand(ILI9488_MADCTL);
    rotation = m % 8;
    switch (rotation) {
        case 0: // Portrait
            writeData(MADCTL_BGR | MADCTL_MX);
            width = ILI9488_TFTWIDTH;
            height = ILI9488_TFTHEIGHT;
            break;
        case 1: // Landscape (Portrait + 90)
            writeData(MADCTL_BGR | MADCTL_MV);
            width = ILI9488_TFTHEIGHT;
            height = ILI9488_TFTWIDTH;
            break;
        case 2: // Inverter portrait
            writeData(MADCTL_BGR | MADCTL_MY);
            width = ILI9488_TFTWIDTH;
            height = ILI9488_TFTHEIGHT;
            break;
        case 3: // Inverted landscape
            writeData(MADCTL_BGR | MADCTL_MV | MADCTL_MX | MADCTL_MY);
            width = ILI9488_TFTHEIGHT;
            height = ILI9488_TFTWIDTH;
            break;
        case 4: // Portrait
            writeData(MADCTL_BGR | MADCTL_MX | MADCTL_MY);
            width = ILI9488_TFTWIDTH;
            height = ILI9488_TFTHEIGHT;
            break;
        case 5: // Landscape (Portrait + 90)
            writeData(MADCTL_BGR | MADCTL_MV | MADCTL_MX);
            width = ILI9488_TFTHEIGHT;
            height = ILI9488_TFTWIDTH;
            break;
        case 6: // Inverter portrait
            writeData(MADCTL_BGR);
            width = ILI9488_TFTWIDTH;
            height = ILI9488_TFTHEIGHT;
            break;
        case 7: // Inverted landscape
            writeData(MADCTL_BGR | MADCTL_MV | MADCTL_MY);
            width = ILI9488_TFTHEIGHT;
            height = ILI9488_TFTWIDTH;
            break;
    }
}

void invertDisplay(BOOL i)
{
    if(i)
        writeCommand(ILI9488_INVON);
    else
        writeCommand(ILI9488_INVOFF);
    
    LCD_CS = 1;
}

void setTextSize(uint8_t s)
{
    if(s > 0)
        textSize = s;
    else
        textSize = 1;
}
int16_t drawChar2(uint16_t x, uint16_t y, unsigned char c, uint16_t color,
        uint16_t bg, uint8_t size)
{
      if (_vpOoB) return;

  int32_t xd = x + _xDatum;
  int32_t yd = y + _yDatum;

  if (c < 32) return;
  
//  if ((xd >= _vpW)                 || // Clip right
//     ( yd >= _vpH)                 || // Clip bottom
//     ((xd + 6 * size - 1) < _vpX)  || // Clip left
//     ((yd + 8 * size - 1) < _vpY))    // Clip top
//    return;
  
  bool fillbg = (bg != color);
  bool clip = xd < _vpX || xd + 6  * textSize >= _vpW || yd < _vpY || yd + 8 * textSize >= _vpH;
  uint8_t i,j,k;
    if ((size==1) && fillbg && !clip) {
    uint8_t column[6];
    uint8_t mask = 0x1;

    setAddrWindow(xd, yd, xd+5, yd+8);

    for (i = 0; i < 5; i++ ) 
        column[i] = font[c * 5 + i];
    column[5] = 0;

    for ( j = 0; j < 8; j++) {
      for ( k = 0; k < 5; k++ ) {
        if (column[k] & mask) 
        {   
            pushColors(color, 1);
        }
        else 
        {
            pushColors(bg, 1);
        }
      }
      mask <<= 1;
      pushColors(bg, 1);
    }
  }
  else {

    for ( i = 0; i < 6; i++ ) {
      uint8_t line;
      if (i == 5)
        line = 0x0;
      else
        line = font[c * 5 + i];

      if (size == 1 && !fillbg) { // default size
        for ( j = 0; j < 8; j++) {
          if (line & 0x1) 
              drawPixel(x + i, y + j, color);
          line >>= 1;
        }
      }
      else {  // big size or clipped
        for ( j = 0; j < 8; j++) {
          if (line & 0x1) 
              fillRect(x + (i * size), y + (j * size), size, size, color);
          else if (fillbg) 
              fillRect(x + i * size, y + j * size, size, size, bg);
          line >>= 1;
        }
      }
    }
  }
  return 6 * textSize;
}

int16_t textWidth(char *string)
{
    int32_t str_width = 0;
    while (*string++) 
        str_width += 6;
    
    return str_width * textSize;
}
uint16_t drawString(char *string, uint16_t poX, uint16_t poY)
{
  int16_t sumX = 0;
  uint8_t padding = 1, baseline = 0;
  uint16_t cwidth = textWidth(string); // Find the pixel width of the string in the font
  uint16_t cheight = 8 * textSize;
  
  if (textdatum || padX) {

    switch(textdatum) {
      case TC_DATUM:
        poX -= cwidth/2;
        padding += 1;
        break;
      case TR_DATUM:
        poX -= cwidth;
        padding += 2;
        break;
      case ML_DATUM:
        poY -= cheight/2;
        //padding += 0;
        break;
      case MC_DATUM:
        poX -= cwidth/2;
        poY -= cheight/2;
        padding += 1;
        break;
      case MR_DATUM:
        poX -= cwidth;
        poY -= cheight/2;
        padding += 2;
        break;
      case BL_DATUM:
        poY -= cheight;
        //padding += 0;
        break;
      case BC_DATUM:
        poX -= cwidth/2;
        poY -= cheight;
        padding += 1;
        break;
      case BR_DATUM:
        poX -= cwidth;
        poY -= cheight;
        padding += 2;
        break;
      case L_BASELINE:
        poY -= baseline;
        //padding += 0;
        break;
      case C_BASELINE:
        poX -= cwidth/2;
        poY -= baseline;
        padding += 1;
        break;
      case R_BASELINE:
        poX -= cwidth;
        poY -= baseline;
        padding += 2;
        break;
    }
  }
    int8_t xo = 0;
  uint16_t len = strlen(string);
  uint16_t n = 0;
  
  while (n < len) {
      sumX += drawChar2(poX+sumX, poY, string[n++], textColor, textBgColor, textSize);
    }

  if((padX>cwidth) && (textColor!=textBgColor)) {
    int16_t padXc = poX+cwidth+xo;
  
  switch(padding) {
      case 1:
        fillRect(padXc,poY,padX-cwidth,cheight, textBgColor);
        break;
      case 2:
        fillRect(padXc,poY,(padX-cwidth)>>1,cheight, textBgColor);
        padXc = poX - ((padX-cwidth)>>1);
        fillRect(padXc,poY,(padX-cwidth)>>1,cheight, textBgColor);
        break;
      case 3:
        if (padXc>padX) padXc = padX;
        fillRect(poX + cwidth - padXc,poY,padXc-cwidth,cheight, textBgColor);
        break;
    }
  }
  
  return sumX;
}
void drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color,
              uint16_t bg, uint8_t size_x, uint8_t size_y) 
{
    uint8_t i;
    uint8_t j;
    if ((x >= width)                ||  // Clip right
        (y >= height)               ||  // Clip bottom
        ((x + 6 * size_x - 1) < 0)  ||  // Clip left
        ((y + 8 * size_y - 1) < 0))     // Clip top
      return;


    for (i = 0; i < 5; i++) { // Char bitmap = 5 columns
      uint8_t line = font[c * 5 + i];
      for (j = 0; j < 8; j++, line >>= 1) {
        if (line & 1) {
          if (size_x == 1 && size_y == 1)
            drawPixel(x + i, y + j, color);
          else
            fillRect(x + i * size_x, y + j * size_y, size_x, size_y,
                          color);
        } else if (bg != color) {
          if (size_x == 1 && size_y == 1)
            drawPixel(x + i, y + j, bg);
          else
            fillRect(x + i * size_x, y + j * size_y, size_x, size_y, bg);
        }
      }
    }
    if (bg != color) { // If opaque, draw vertical line for last column
      if (size_x == 1 && size_y == 1)
        drawFastVLine(x + 5, y, 8, bg);
      else
        fillRect(x + 5 * size_x, y, size_x, 8 * size_y, bg);
    }

}
void writeChar(unsigned char c)
{   
	if (c == '\n')
	{
		cursor_y += textSize * 8;
		cursor_x = 0;
	}
	else
	{
        drawChar(cursor_x, cursor_y, c, textColor, textBgColor, textSize, textSize);
		cursor_x += textSize * 6;
		if (wrap && (cursor_x > (getWidth() - textSize * 6)))
		{
			cursor_y += textSize * 8;
			cursor_x = 0;
		}
	}
}
void writeString(char *s, uint16_t xPos, uint16_t yPos)
{
    if(s == NULL)
        return;
    uint32_t i = 0;
    setCursor(xPos, yPos);
    while(s[i] != '\0')
       writeChar(s[i++]);
}

void setCursor(uint16_t x, uint16_t y) 
{
    cursor_x = x;
    cursor_y = y;
}

void setTextColor(uint16_t c, uint16_t bg) 
{
    textColor = c;
    textBgColor = bg;
}

void setTextWrap(bool w) 
{
    wrap = w;
}
uint16_t getTextColor()
{
    return textColor;
}
uint16_t getTextBgColor()
{
    return textBgColor;
}
uint16_t getWidth(void) 
{
    if (_vpDatum) 
        return _xWidth;
    return width;

};

uint16_t getHeight(void) 
{
    if (_vpDatum) 
        return _yHeight;
    return height;
}

uint8_t getRotation(void) 
{
    return rotation;
}

int16_t getCursorX(void) 
{
    return cursor_x;
}

int16_t getCursorY(void) 
{
    return cursor_y;
};


void setViewport(int32_t x, int32_t y, int32_t w, int32_t h, bool vpDatum)
{
  // Viewport metrics (not clipped)
  _xDatum  = x; // Datum x position in screen coordinates
  _yDatum  = y; // Datum y position in screen coordinates
  _xWidth  = w; // Viewport width
  _yHeight = h; // Viewport height

  // Full size default viewport
  _vpDatum = false; // Datum is at top left corner of screen (true = top left of viewport)
  _vpOoB   = false; // Out of Bounds flag (true is all of viewport is off screen)
  _vpX = 0;         // Viewport top left corner x coordinate
  _vpY = 0;         // Viewport top left corner y coordinate
  _vpW = getWidth();   // Equivalent of TFT width  (Nb: viewport right edge coord + 1)
  _vpH = getHeight();  // Equivalent of TFT height (Nb: viewport bottom edge coord + 1)

  // Clip viewport to screen area
  if (x<0) { w += x; x = 0; }
  if (y<0) { h += y; y = 0; }
  if ((x + w) > getWidth() ) { w = getWidth()  - x; }
  if ((y + h) > getHeight() ) { h = getHeight() - y; }

  // Check if viewport is entirely out of bounds
  if (w < 1 || h < 1)
  {
    // Set default values and Out of Bounds flag in case of error
    _xDatum = 0;
    _yDatum = 0;
    _xWidth  = getWidth();
    _yHeight = getHeight();
    _vpOoB = true;      // Set Out of Bounds flag to inhibit all drawing
    return;
  }

  if (!vpDatum)
  {
    _xDatum = 0; // Reset to top left of screen if not using a viewport datum
    _yDatum = 0;
    _xWidth  = getWidth();
    _yHeight = getHeight();
  }

  // Store the clipped screen viewport metrics and datum position
  _vpX = x;
  _vpY = y;
  _vpW = x + w;
  _vpH = y + h;
  _vpDatum = vpDatum;

}

bool checkViewport(int32_t x, int32_t y, int32_t w, int32_t h)
{
  if (_vpOoB) return false;
  x+= _xDatum;
  y+= _yDatum;

  if ((x >= _vpW) || (y >= _vpH)) return false; 

  int32_t dx = 0;
  int32_t dy = 0;
  int32_t dw = w;
  int32_t dh = h;

  if (x < _vpX) { dx = _vpX - x; dw -= dx; x = _vpX; }
  if (y < _vpY) { dy = _vpY - y; dh -= dy; y = _vpY; }

  if ((x + dw) > _vpW ) dw = _vpW - x;
  if ((y + dh) > _vpH ) dh = _vpH - y;

  if (dw < 1 || dh < 1) return false;

  return true;
}

void resetViewport(void)
{
  // Reset viewport to the whole screen (or sprite) area
  _vpDatum = false;
  _vpOoB   = false;
  _xDatum = 0;
  _yDatum = 0;
  _vpX = 0;
  _vpY = 0;
  _vpW = getWidth();
  _vpH = getHeight();
  _xWidth  = getWidth();
  _yHeight = getHeight();
}

int32_t  getViewportX(void)
{
  return _xDatum;
}

/***************************************************************************************
** Function name:           getViewportY
** Description:             Get y position of the viewport datum
***************************************************************************************/
int32_t  getViewportY(void)
{
  return _yDatum;
}

/***************************************************************************************
** Function name:           getViewportWidth
** Description:             Get width of the viewport
***************************************************************************************/
int32_t getViewportWidth(void)
{
  return _xWidth;
}

/***************************************************************************************
** Function name:           getViewportHeight
** Description:             Get height of the viewport
***************************************************************************************/
int32_t getViewportHeight(void)
{
  return _yHeight;
}

/***************************************************************************************
** Function name:           getViewportDatum
** Description:             Get datum flag of the viewport (true = viewport corner)
***************************************************************************************/
bool  getViewportDatum(void)
{
  return _vpDatum;
}

/***************************************************************************************
** Function name:           frameViewport
** Description:             Draw a frame inside or outside the viewport of width w
***************************************************************************************/
void frameViewport(uint16_t color, int32_t w)
{
  // Save datum position
  bool _dT = _vpDatum;

  // If w is positive the frame is drawn inside the viewport
  // a large positive width will clear the screen inside the viewport
  if (w>0)
  {
    // Set vpDatum true to simplify coordinate derivation
    _vpDatum = true;
    fillRect(0, 0, _vpW - _vpX, w, color);                // Top
    fillRect(0, w, w, _vpH - _vpY - w - w, color);        // Left
    fillRect(_xWidth - w, w, w, _yHeight - w - w, color); // Right
    fillRect(0, _yHeight - w, _xWidth, w, color);         // Bottom
  }
  else
  // If w is negative the frame is drawn outside the viewport
  // a large negative width will clear the screen outside the viewport
  {
    w = -w;
    
    // Save old values
    int32_t _xT = _vpX; _vpX = 0;
    int32_t _yT = _vpY; _vpY = 0;
    int32_t _wT = _vpW;
    int32_t _hT = _vpH;

    // Set vpDatum false so frame can be drawn outside window
    _vpDatum = false; // When false the full width and height is accessed
    _vpH = getHeight();
    _vpW = getWidth();

    // Draw frame
    fillRect(_xT - w - _xDatum, _yT - w - _yDatum, _wT - _xT + w + w, w, color); // Top
    fillRect(_xT - w - _xDatum, _yT - _yDatum, w, _hT - _yT, color);             // Left
    fillRect(_wT - _xDatum, _yT - _yDatum, w, _hT - _yT, color);                 // Right
    fillRect(_xT - w - _xDatum, _hT - _yDatum, _wT - _xT + w + w, w, color);     // Bottom

    // Restore old values
    _vpX = _xT;
    _vpY = _yT;
    _vpW = _wT;
    _vpH = _hT;
  }

  // Restore vpDatum
  _vpDatum = _dT;
}
uint8_t getTextDatum(void)
{
  return textdatum;
}
void setTextDatum(uint8_t d)
{
  textdatum = d;
}

/***************************************************************************************
** Function name:           setTextPadding
** Description:             Define padding width (aids erasing old text and numbers)
***************************************************************************************/
void setTextPadding(uint16_t x_width)
{
  padX = x_width;
}

/***************************************************************************************
** Function name:           setTextPadding
** Description:             Define padding width (aids erasing old text and numbers)
***************************************************************************************/
uint16_t getTextPadding(void)
{
  return padX;
}

/***************************************************************************************
** Function name:           alphaBlend
** Description:             Blend 16bit foreground and background
*************************************************************************************x*/
uint16_t alphaBlend(uint8_t alpha, uint16_t fgc, uint16_t bgc)
{
  // For speed use fixed point maths and rounding to permit a power of 2 division
  uint16_t fgR = ((fgc >> 10) & 0x3E) + 1;
  uint16_t fgG = ((fgc >>  4) & 0x7E) + 1;
  uint16_t fgB = ((fgc <<  1) & 0x3E) + 1;

  uint16_t bgR = ((bgc >> 10) & 0x3E) + 1;
  uint16_t bgG = ((bgc >>  4) & 0x7E) + 1;
  uint16_t bgB = ((bgc <<  1) & 0x3E) + 1;

  // Shift right 1 to drop rounding bit and shift right 8 to divide by 256
  uint16_t r = (((fgR * alpha) + (bgR * (255 - alpha))) >> 9);
  uint16_t g = (((fgG * alpha) + (bgG * (255 - alpha))) >> 9);
  uint16_t b = (((fgB * alpha) + (bgB * (255 - alpha))) >> 9);

  // Combine RGB565 colours into 16 bits
  //return ((r&0x18) << 11) | ((g&0x30) << 5) | ((b&0x18) << 0); // 2 bit greyscale
  //return ((r&0x1E) << 11) | ((g&0x3C) << 5) | ((b&0x1E) << 0); // 4 bit greyscale
  return (r << 11) | (g << 5) | (b << 0);
}


/***************************************************************************************
** Function name:           fillRectHGradient
** Description:             draw a filled rectangle with a horizontal colour gradient
***************************************************************************************/
void fillRectHGradient(int16_t x, int16_t y, int16_t w, int16_t h, uint32_t color1, uint32_t color2)
{
  if (_vpOoB) return;

  x+= _xDatum;
  y+= _yDatum;

  // Clipping
//  if ((x >= _vpW) || (y >= _vpH)) return;
//
//  if (x < _vpX) { w += x - _vpX; x = _vpX; }
//  if (y < _vpY) { h += y - _vpY; y = _vpY; }
//
//  if ((x + w) > _vpW) w = _vpW - x;
//  if ((y + h) > _vpH) h = _vpH - y;

  if ((w < 1) || (h < 1)) return;

  float delta = -255.0/w;
  float alpha = 255.0;
  uint32_t color = color1;

  while (w--) {
    drawFastVLine(x++, y, h, color);
    alpha += delta;
    color = alphaBlend((uint8_t)alpha, color1, color2);
  }
}

/***************************************************************************************
** Function name:           fillRectVGradient
** Description:             draw a filled rectangle with a vertical colour gradient
***************************************************************************************/
void fillRectVGradient(int16_t x, int16_t y, int16_t w, int16_t h, uint32_t color1, uint32_t color2)
{
  if (_vpOoB) return;

  x+= _xDatum;
  y+= _yDatum;

  // Clipping
//  if ((x >= _vpW) || (y >= _vpH)) return;
//
//  if (x < _vpX) { w += x - _vpX; x = _vpX; }
//  if (y < _vpY) { h += y - _vpY; y = _vpY; }
//
//  if ((x + w) > _vpW) w = _vpW - x;
//  if ((y + h) > _vpH) h = _vpH - y;

  if ((w < 1) || (h < 1)) return;

  float delta = -255.0/h;
  float alpha = 255.0;
  uint32_t color = color1;

  while (h--) {
    drawFastHLine(x, y++, w, color);
    alpha += delta;
    color = alphaBlend((uint8_t)alpha, color1, color2);
  }
}