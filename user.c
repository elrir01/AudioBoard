/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#ifdef __XC32
    #include <xc.h>          /* Defines special funciton registers, CP0 regs  */
#endif
#define _SUPPRESS_PLIB_WARNING
#define _DISABLE_OPENADC10_CONFIGPORT_WARNING
#include <plib.h>            /* Include to use PIC32 peripheral libraries     */
#include <stdint.h>          /* For uint32_t definition                       */
#include <stdbool.h>         /* For true/false definition                     */
#include <stdio.h>
#include <proc/p32mx795f512h.h>
#include "user.h"            /* variables/params used by user.c               */
#include "system.h"
#define TOGGLE_PER_SEC_MS 40000
#define TOGGLE_PER_SEC_US 40
/******************************************************************************/
/* User Functions                                                             */
/******************************************************************************/

uint32_t elTime = 0; // elapsed time in ms
void InitApp(void)
{
    /* Setup analog functionality and port direction */
    //ALL Digital port
    AD1CON1bits.ADON = 0;
    AD1PCFG = 0xFFFF;
    //Disable ChangeOnNotification 
    CNCONbits.ON = 0;
    //Disable ChangeOnNotification 
    CNEN = 0;
    //Disable PullUp/PullDown 
    CNPUE = 0;
    
    TRISB = 0x0;
    LATB = 0x0;
    TRISC = 0x0;
    LATC = 0x0;
    TRISD = 0x0;
    LATD = 0x0;
    TRISE = 0x0;
    LATE = 0x0;
    TRISF = 0x0;
    LATF = 0x0;
    TRISG = 0x0;
    LATG = 0x0;
//    TRISFbits.TRISF5 = 0;
//    LATFbits.LATF5 = 0;
    TRISBbits.TRISB15 = 0;  // bme280_CS
    LATBbits.LATB15   = 1;    // active low default high
    TRISBbits.TRISB14 = 0;  //SCK4 output
    TRISFbits.TRISF4  = 1;  //SDI4 input
    TRISFbits.TRISF5  = 0;  //SDO4 output
    
    TRISEbits.TRISE0 = 0;   //RX_IND MFB        BM64
    LATEbits.LATE0   = 0;
    TRISEbits.TRISE1 = 1;   //TX_IND P3_7       BM64
    TRISEbits.TRISE2 = 0;   //RST_N active low  BM64
    LATEbits.LATE2   = 1;   //active low default high
    
    TRISEbits.TRISE5 = 0;   //LCD_RST
    TRISEbits.TRISE6 = 0;   //LCD_DC
    TRISEbits.TRISE7 = 0;   //LCD_CS
    TRISEbits.TRISE7 = 1;   //active low default high
    TRISGbits.TRISG9 = 0;   //TOUCH_CS output
    LATGbits.LATG9   = 1;   //active low default high
    TRISDbits.TRISD6 = 0;   //BME_CS
    LATDbits.LATD6   = 1;   //active low default high
    TRISGbits.TRISG6 = 0;   //SCK2 output
    TRISGbits.TRISG7 = 1;   //SDI2 input
    TRISGbits.TRISG8 = 0;   //SDO2 output
    
//    mConfigIntCoreTimer(CT_INT_ON | CT_INT_PRIOR_0);
//    mEnableIntCoreTimer();
//    OpenCoreTimer(0);

    /* Initialize peripherals */
}
void delay_ms(uint16_t t)
{
    uint32_t ms = TOGGLE_PER_SEC_MS * t;
    _CP0_SET_COUNT(0);
    while(ms > _CP0_GET_COUNT());
}

void delay_us(uint16_t t)
{
    uint32_t us = TOGGLE_PER_SEC_US * t;
    _CP0_SET_COUNT(0);
    while(us > _CP0_GET_COUNT());
}
uint32_t millis()
{
    return elTime;
}
void UART1Setup(void)
{   
    UARTConfigure(UART1,UART_ENABLE_PINS_TX_RX_ONLY);
    UARTSetFifoMode(UART1, UART_INTERRUPT_ON_TX_NOT_FULL | UART_INTERRUPT_ON_RX_NOT_EMPTY);
    UARTSetLineControl(UART1, UART_DATA_SIZE_8_BITS | UART_PARITY_NONE | UART_STOP_BITS_1);
    UARTSetDataRate(UART1, SYS_FREQ, 115200);
    UARTEnable(UART1, UART_ENABLE_FLAGS(UART_PERIPHERAL | UART_RX | UART_TX));
}

uint8_t UART1Receive(void)
{
    uint8_t data;
    while(!UARTReceivedDataIsAvailable(UART1))
        ;
    data = UARTGetDataByte(UART1);
    return data;
}

void UART1Transmit(uint8_t data)
{
    while(!UARTTransmitterIsReady(UART1))
        ;
    UARTSendDataByte(UART1, data);
    while(!UARTTransmissionHasCompleted(UART1))
        ;
}

void UART1HexTransmit(uint8_t val)
{
    unsigned char upperNibble = (val & 0xF0) >> 4;
    unsigned char lowerNibble = val & 0x0F;
    upperNibble += upperNibble > 9 ? 'A' - 10 : '0';
    lowerNibble += lowerNibble > 9 ? 'A' - 10 : '0';
    UART1Transmit(upperNibble);
    UART1Transmit(lowerNibble);
//  printf("\n\r");
    UART1Transmit(' ');
}
void __ISR(_TIMER_1_VECTOR, ipl2auto)_Timer1Handler(void)
{
    elTime++;
    mT1ClearIntFlag();
    
}
