/* 
 * File:   XPT2046_driver.h
 * Author: Mattia
 *
 * Created on 31 maggio 2022, 13.55
 */

#ifndef XPT2046_DRIVER_H
#define	XPT2046_DRIVER_H
#ifdef __XC32
    #include <xc.h>          /* Defines special funciton registers, CP0 regs  */
#endif
#define _SUPPRESS_PLIB_WARNING
#define _DISABLE_OPENADC10_CONFIGPORT_WARNING
#include <plib.h>            /* Include to use PIC32 peripheral libraries     */
#include <stdint.h>          /* For uint32_t definition                       */
#include <stdbool.h>         /* For true/false definition                     */
#include "../../user.h"
#include "ILI9488_driver.h"
#define  TP_CS      LATGbits.LATG9 

uint8_t  getTouchRaw(uint16_t *x, uint16_t *y);
       // Get raw z (i.e. pressure) ADC value from touch controller
uint16_t getTouchRawZ(void);
       // Convert raw x,y values to calibrated and correctly rotated screen coordinates
void     convertRawXY(uint16_t *x, uint16_t *y);
       // Get the screen touch coordinates, returns true if screen has been touched
       // if the touch coordinates are off screen then x and y are not updated
       // The returned value can be treated as a bool type, false or 0 means touch not detected
       // In future the function may return an 8 "quality" (jitter) value.
//threshold  = 600
uint8_t  getTouch(uint16_t *x, uint16_t *y, uint16_t threshold);

       // Run screen calibration and test, report calibration values to the serial port
void     calibrateTouch(uint32_t *data, uint32_t color_fg, uint32_t color_bg, uint8_t size);
       // Set the screen calibration values
void     setTouch(uint32_t *data);
void    saveCal(uint32_t *calData, uint32_t nvmAddr);
void    loadCal(uint32_t *buffer, uint32_t nvmAddr);
static void beginTouchRW();
static void endTouchRW();

       // Private function to validate a touch, allow settle time and reduce spurious coordinates
//threshold  = 600
 uint8_t  validTouch(uint16_t *x, uint16_t *y, uint16_t threshold);
static uint8_t SPI2Transfer(uint8_t data);

       // Initialise with example calibration values so processor does not crash if setTouch() not called in setup()

//uint16_t touchCalibration_x0, touchCalibration_x1, touchCalibration_y0, touchCalibration_y1;
//uint8_t  touchCalibration_rotate, touchCalibration_invert_x, touchCalibration_invert_y;

uint16_t _pressX, _pressY;  // For future use (last sampled calibrated coordinates)
#endif	/* XPT2046_DRIVER_H */

