
#include "../../user.h"
#include "XPT2046_driver.h"
/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */

/*  A brief description of a section can be given directly below the section
    banner.
 */

uint32_t touchCalibration_x0 = 289, touchCalibration_x1 = 3646, touchCalibration_y0 = 248, touchCalibration_y1 = 3481;
uint32_t  touchCalibration_rotate = 1, touchCalibration_invert_x = 0, touchCalibration_invert_y = 1;
uint32_t _pressTime = 0;        // Press and hold time-out
/* ************************************************************************** */
/* ************************************************************************** */
// Section: Local Functions                                                   */
/* ************************************************************************** */
/* ************************************************************************** */
static void beginTouchRW()
{   //lower speed to talk to TOUCH
    SpiChnSetBrg(2,15); // SPIBRG = 15 for 2.5MHz @ 80MHz
    TP_CS = 0;
}
static void endTouchRW()
{   //restore previous speed to talk to TFT
    SpiChnSetBrg(2,1); // SPIBRG = 1 for 20MHz @ 80MHz
    TP_CS = 1;
}
static uint8_t SPI2Transfer(uint8_t data)
{
    SpiChnPutC(2, data);
    return SpiChnGetC(2);
}
#define _RAWERR 20 // Deadband error allowed in successive position samples
 uint8_t  validTouch(uint16_t *x, uint16_t *y, uint16_t threshold)
{
    uint16_t x_tmp, y_tmp, x_tmp2, y_tmp2;
      // Wait until pressure stops increasing to debounce pressure
    uint16_t z1 = 1;
    uint16_t z2 = 0;
    while(z1 > z2)
    {
        z2 = z1;
        z1 = getTouchRawZ();
        delay_ms(1);
    }
    if(z1 <= threshold)
        return false;
    
    getTouchRaw(&x_tmp, &y_tmp);
    
    delay_ms(1);// small delay before next sample
    if(getTouchRawZ() <= threshold)
        return false;
    
    delay_ms(2);// small delay before next sample
    getTouchRaw(&x_tmp2, &y_tmp2);
    
    if(abs(x_tmp - x_tmp2) > _RAWERR)
        return false;
    if(abs(y_tmp - y_tmp2) > _RAWERR)
        return false;
    
    *x = x_tmp;
    *y = y_tmp;
    
    return true;
}
/* ************************************************************************** */
/* ************************************************************************** */
// Section: Interface Functions                                               */
/* ************************************************************************** */
/* ************************************************************************** */
/***************************************************************************************
** Function name:           getTouchRaw
** Description:             read raw touch position.  Always returns true.
***************************************************************************************/
uint8_t  getTouchRaw(uint16_t *x, uint16_t *y)
{
    uint16_t temp;
    
    beginTouchRW();
    SPI2Transfer(0xD0);                    // Start new YP conversion
    SPI2Transfer(0);// Read first 8 bits
    SPI2Transfer(0xD0);                    // Read last 8 bits and start new YP conversion
    SPI2Transfer(0);                       // Read first 8 bits
    SPI2Transfer(0xD0);                    // Read last 8 bits and start new YP conversion
    SPI2Transfer(0);                       // Read first 8 bits
    SPI2Transfer(0xD0);                    // Read last 8 bits and start new YP conversion

    temp = SPI2Transfer(0);                // Read first 8 bits
    temp = temp << 5;
    temp = temp | (0x1F & (SPI2Transfer(0x90) >> 3)); // Read last 8 bits and start new XP conversion
    
    *x = temp;
    
      // Start XP sample request for y position, read 4 times and keep last sample
    SPI2Transfer(0);                       // Read first 8 bits
    SPI2Transfer(0x90);                    // Read last 8 bits and start new XP conversion
    SPI2Transfer(0);                       // Read first 8 bits
    SPI2Transfer(0x90);                    // Read last 8 bits and start new XP conversion
    SPI2Transfer(0);                       // Read first 8 bits
    SPI2Transfer(0x90);                    // Read last 8 bits and start new XP conversion

    temp = SPI2Transfer(0);                 // Read first 8 bits
    temp = temp <<5;
    temp = temp | (0x1F & (SPI2Transfer(0)>>3));    // Read last 8 bits

    *y = temp;
    
    endTouchRW();
    
    return true;
}
uint16_t getTouchRawZ()
{
    beginTouchRW();
    int16_t tz = 0xFFF;
    uint8_t tzLow, tzHigh;
    
    SPI2Transfer(0xB0);          //Start new Z1 conversion
    tzLow = SPI2Transfer(0);     //Read first 8 bits Z1
    tzHigh = SPI2Transfer(0xC0); //Read last 8 bits Z1 and start Z2 conversion
    tz += (tzHigh | tzLow) >> 3;
    tzLow = SPI2Transfer(0);     //Read first 8 bit Z2
    tzHigh = SPI2Transfer(0);    //Read last 8 bits Z2
    tz -= (tzHigh | tzLow) >> 3;
    endTouchRW();
    return (uint16_t)tz;
}

#define Z_THRESHOLD 4090 // Touch pressure threshold for validating touches
uint8_t getTouch(uint16_t *x, uint16_t *y, uint16_t threshold)
{
    uint16_t x_tmp, y_tmp;
    
    if(threshold < 20)
        threshold = 20;
    if(_pressTime > millis())
        threshold = 20;
    
    uint8_t n = 5;
    uint8_t valid = 0;
    
    while(n--)
    {
        if(validTouch(&x_tmp, &y_tmp, threshold))
            valid++;
    }
    if(valid < 1)
    {
        _pressTime = 0;
        return false;
    }
    _pressTime = millis() + 50;
    
    convertRawXY(&x_tmp, &y_tmp);
    if(x_tmp >= getWidth() || y_tmp >= getHeight())
        return false;
    
    _pressX = x_tmp;
    _pressY = y_tmp;
    *x = _pressX;
    *y = _pressY;
    return valid;
}

void convertRawXY(uint16_t *x, uint16_t *y)
{
    uint16_t x_tmp = *x, y_tmp = *y, xx, yy;

    if(!touchCalibration_rotate)
    {
        xx=(x_tmp-touchCalibration_x0)*getWidth()/touchCalibration_x1;
        yy=(y_tmp-touchCalibration_y0)*getHeight()/touchCalibration_y1;
        if(touchCalibration_invert_x)
            xx = getWidth() - xx;
        if(touchCalibration_invert_y)
            yy = getHeight() - yy;
    } 
    else 
    {
        xx=(y_tmp-touchCalibration_x0)*getWidth()/touchCalibration_x1;
        yy=(x_tmp-touchCalibration_y0)*getHeight()/touchCalibration_y1;
        if(touchCalibration_invert_x)
            xx = getWidth() - xx;
        if(touchCalibration_invert_y)
            yy = getHeight() - yy;
    }
    
    *x = xx;
    *y = yy;
}

void calibrateTouch(uint32_t *parameters, uint32_t color_fg, uint32_t color_bg, uint8_t size){
  int32_t values[] = {0,0,0,0,0,0,0,0};
  uint16_t x_tmp, y_tmp;
  uint8_t i, j;



  for( i = 0; i<4; i++)
  {
    fillRect(0, 0, size+1, size+1, color_bg);
    fillRect(0, getHeight()-size-1, size+1, size+1, color_bg);
    fillRect(getWidth()-size-1, 0, size+1, size+1, color_bg);
    fillRect(getWidth()-size-1, getHeight()-size-1, size+1, size+1, color_bg);

    if (i == 5) break; // used to clear the arrows
    
    switch (i) {
      case 0: // up left
        drawLine(0, 0, 0, size, color_fg);
        drawLine(0, 0, size, 0, color_fg);
        drawLine(0, 0, size , size, color_fg);
        break;
      case 1: // bot left
        drawLine(0, getHeight()-size-1, 0, getHeight()-1, color_fg);
        drawLine(0, getHeight()-1, size, getHeight()-1, color_fg);
        drawLine(size, getHeight()-size-1, 0, getHeight()-1 , color_fg);
        break;
      case 2: // up right
        drawLine(getWidth()-size-1, 0, getWidth()-1, 0, color_fg);
        drawLine(getWidth()-size-1, size, getWidth()-1, 0, color_fg);
        drawLine(getWidth()-1, size, getWidth()-1, 0, color_fg);
        break;
      case 3: // bot right
        drawLine(getWidth()-size-1, getHeight()-size-1, getWidth()-1, getHeight()-1, color_fg);
        drawLine(getWidth()-1, getHeight()-1-size, getWidth()-1, getHeight()-1, color_fg);
        drawLine(getWidth()-1-size, getHeight()-1, getWidth()-1, getHeight()-1, color_fg);
        break;
      }

    // user has to get the chance to release
    if(i>0) 
        delay_ms(1000);

    for(j= 0; j<8; j++)
    {
      // Use a lower detect threshold as corners tend to be less sensitive
      while(!validTouch(&x_tmp, &y_tmp, Z_THRESHOLD));
      values[i*2  ] += x_tmp;
      values[i*2+1] += y_tmp;
      }
    values[i*2  ] /= 8;
    values[i*2+1] /= 8;
  }


  // from case 0 to case 1, the y value changed. 
  // If the measured delta of the touch x axis is bigger than the delta of the y axis, the touch and TFT axes are switched.
  touchCalibration_rotate = false;
  if(abs(values[0]-values[2]) > abs(values[1]-values[3])){
    touchCalibration_rotate = true;
    touchCalibration_x0 = (values[1] + values[3])/2; // calc min x
    touchCalibration_x1 = (values[5] + values[7])/2; // calc max x
    touchCalibration_y0 = (values[0] + values[4])/2; // calc min y
    touchCalibration_y1 = (values[2] + values[6])/2; // calc max y
  } else {
    touchCalibration_x0 = (values[0] + values[2])/2; // calc min x
    touchCalibration_x1 = (values[4] + values[6])/2; // calc max x
    touchCalibration_y0 = (values[1] + values[5])/2; // calc min y
    touchCalibration_y1 = (values[3] + values[7])/2; // calc max y
  }

  // in addition, the touch screen axis could be in the opposite direction of the TFT axis
  touchCalibration_invert_x = false;
  if(touchCalibration_x0 > touchCalibration_x1){
    values[0]=touchCalibration_x0;
    touchCalibration_x0 = touchCalibration_x1;
    touchCalibration_x1 = values[0];
    touchCalibration_invert_x = true;
  }
  touchCalibration_invert_y = false;
  if(touchCalibration_y0 > touchCalibration_y1){
    values[0]=touchCalibration_y0;
    touchCalibration_y0 = touchCalibration_y1;
    touchCalibration_y1 = values[0];
    touchCalibration_invert_y = true;
  }

  // pre calculate
  touchCalibration_x1 -= touchCalibration_x0;
  touchCalibration_y1 -= touchCalibration_y0;

  if(touchCalibration_x0 == 0) touchCalibration_x0 = 1;
  if(touchCalibration_x1 == 0) touchCalibration_x1 = 1;
  if(touchCalibration_y0 == 0) touchCalibration_y0 = 1;
  if(touchCalibration_y1 == 0) touchCalibration_y1 = 1;

  // export parameters, if pointer valid
  if(parameters != NULL){
    parameters[0] = touchCalibration_x0;
    parameters[1] = touchCalibration_x1;
    parameters[2] = touchCalibration_y0;
    parameters[3] = touchCalibration_y1;
    parameters[4] = (touchCalibration_rotate | (touchCalibration_invert_x <<1) | (touchCalibration_invert_y <<2));
  }
}
void setTouch(uint32_t *parameters){
  touchCalibration_x0 = parameters[0];
  touchCalibration_x1 = parameters[1];
  touchCalibration_y0 = parameters[2];
  touchCalibration_y1 = parameters[3];

  if(touchCalibration_x0 == 0) touchCalibration_x0 = 1;
  if(touchCalibration_x1 == 0) touchCalibration_x1 = 1;
  if(touchCalibration_y0 == 0) touchCalibration_y0 = 1;
  if(touchCalibration_y1 == 0) touchCalibration_y1 = 1;

  touchCalibration_rotate = parameters[4] & 0x1;
  touchCalibration_invert_x = (parameters[4] & 0x2) >> 1;
  touchCalibration_invert_y = (parameters[4] & 0x4) >> 2;
}

void saveCal(uint32_t *calData, uint32_t nvmAddr)
{
    uint8_t i;
    NVMErasePage((void*)nvmAddr);
    for(i = 0; i < 5; i++)
        NVMWriteWord((void*)nvmAddr +i*4,calData[i]);
//    //debug
//    for(i = 0; i < 5; i++)
//            printf("\n\r%d",*(int*)(nvmAddr + i*4));
}

void loadCal(uint32_t *buffer, uint32_t nvmAddr)
{
    uint8_t i;
    for(i = 0; i < 5; i++)
    {
        buffer[i] = *(int*)(nvmAddr +i*4);
    }
}