#ifdef __XC32
    #include <xc.h>          /* Defines special funciton registers, CP0 regs  */
#endif
#define _SUPPRESS_PLIB_WARNING
#define _DISABLE_OPENADC10_CONFIGPORT_WARNING
#include <plib.h>   
#include <stdint.h>         /* For uint32_t definition                        */
#include <stdbool.h>        /* For true/false definition                      */
#include <string.h>
#include <stdio.h>
/******************************************************************************/
/* User Level #define Macros                                                  */
/******************************************************************************/


#define BME_CS      LATBbits.LATB15
#define RX_IND      LATEbits.LATE0
#define TX_IND      LATEbits.LATE1
#define RST_N       LATEbits.LATE2
#define MFB         LATEbits.LATE0
/******************************************************************************/
/* User Function Prototypes                                                    /
/******************************************************************************/

void InitApp(void);         /* I/O and Peripheral Initialization */
//Delay for t ms with core timer 0
void delay_ms(uint16_t t);
//Delay for t us with core timer 0
void delay_us(uint16_t t);
//Return elapsed time from start in ms (max about 50 days)
uint32_t millis();
//Setup UART1 with default parameters, specified after
void UART1Setup(void);
//Receive a byte from UART1
uint8_t UART1Receive(void);
//Transmit a Byte to UART1
void UART1Transmit(uint8_t data);
//Print a HEX value to UART1
void UART1HexTransmit(uint8_t val);


