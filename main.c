/******************************************************************************/
/*  Files to Include                                                          */
/******************************************************************************/
#ifdef __XC32
    #include <xc.h>          /* Defines special funciton registers, CP0 regs  */
#endif
#define _SUPPRESS_PLIB_WARNING
#define _DISABLE_OPENADC10_CONFIGPORT_WARNING
#include <plib.h>           /* Include to use PIC32 peripheral libraries      */
#include <stdint.h>         /* For uint32_t definition                        */
#include <stdbool.h>        /* For true/false definition                      */
#include <string.h>
#include <stdio.h>
#include "system.h"         /* System funct/params, like osc/periph config    */
#include "user.h"           /* User funct/params, such as InitApp             */
#include "Drivers/Display/ILI9488_driver.h"
#include "Drivers/Display/XPT2046_driver.h"

#include "Drivers/Sensor/bme280.h"
#include "Drivers/Audio/BM64.h"
#include "circularBuff.h"
#define NVM_PAGE_SIZE 4096
#define NVM_PROGRAM_PAGE 0x9d010f00
#define THRESHOLD 4090
#define BUFF_SIZE   400 //bytes
//Per utilizzare un "pezzo" di memoria bisogna dichiarare al linker che si intende farlo
// aggiungendo -mreserve=prog@0xStartAddr:0xEndAddr StartAddr ed EndAddr sono gli indirizzi
//fisici in cui e' mappata la memoria programma quindi si guarda alla physical map non alla
//virtual map in riferimento ad DS60001156K-page 52 fig.4-1 ad esempio -mreserve=prog@0x1D010F00:0x1D011F00
// aggiunto in Properties -> XC32(Global Options) -> campo Additional Options.

/******************************************************************************/
/* Global Variable Declaration                                                */
/******************************************************************************/

uint8_t cb_storage[BUFF_SIZE] = {0};
cbuf_handle_t cb = NULL;
BYTE buffer[4096];
unsigned int i;
unsigned int n;
/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/
int32_t main(void)
{

#ifndef PIC32_STARTER_KIT
    DDPCONbits.JTAGEN = 0;
#endif
    __XC_UART = 1;
    uint16_t xTouch = 0, yTouch = 0;
    int32_t adc_T, adc_P, adc_H, compTemp;
    uint32_t compPress, compHum;
    float finalTemp, finalPress, finalHum;
    char stringBuff[100];
    SYSTEMConfig(SYS_FREQ, SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE); 
    /* Initialize I/O and Peripherals for application */
    OpenTimer1(T1_ON | T1_PS_1_256 | T1_SOURCE_INT, 312); //15625 = 50ms count // 312 ~ 1ms
    ConfigIntTimer1(T1_INT_ON | T1_INT_PRIOR_2);
    InitApp();
    
//    INTEnable(INT_SOURCE_UART_RX(UART4), INT_ENABLED);
//    INTSetVectorPriority(INT_VECTOR_UART(UART4), INT_PRIORITY_LEVEL_2);
//    INTSetVectorSubPriority(INT_VECTOR_UART(UART4), INT_SUB_PRIORITY_LEVEL_0);
    
    INTConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);
    INTEnableInterrupts();
    cb = circular_buf_init(cb_storage, BUFF_SIZE);
    SpiChnOpen(4,SPI_OPEN_MSTEN | SPI_OPEN_CKE_REV | SPI_OPEN_MODE8, 8 );
    SpiChnSetBrg(4, 7); // SPIBRG = 7 for 5MHz @ 80MHz Clock
    SpiChnOpen(2,SPI_OPEN_MSTEN | SPI_OPEN_CKE_REV | SPI_OPEN_MODE8, 8 );
    UART1Setup();
    UART_BM64_Setup();
    delay_ms(100);
    
    BME280_init();
    delay_ms(100);
    BME280_read_trim_data();
    
    adc_T = BME280_read_temp();
    compTemp = BME280_compensate_T_int32(adc_T << 4);
    finalTemp = ((float)compTemp) / 100;
    printf("\n\rTemp is : %.2f C\n\r", finalTemp);
    
    adc_P = BME280_read_press();
    compPress = BME280_compensate_P_int64(adc_P << 4);
    finalPress = ((float)compPress) / 256; 
    printf("Press is : %.2f hPa\n\r", finalPress / 100);
    
    adc_H = BME280_read_hum();
    compHum = BME280_compensate_H_int32(adc_H);
    finalHum = ((float)compHum) / 1024;
    printf("Hum is : %.3f %% RH\n\r", finalHum);
    
    GLCD_init();
    fillRect(0, 0, 480, 320, ILI9488_BLACK);
//    if(*(int*)(NVM_PROGRAM_PAGE) != 0xFFFFFFFF)
//    {    loadCal(param, NVM_PROGRAM_PAGE);
//        setTouch(param);
//    }
//    else
//    {
//        calibrateTouch(param, ILI9488_RED, ILI9488_WHITE, 15);
//        saveCal(param, NVM_PROGRAM_PAGE);
//        setTouch(param);
//    }
    setTextColor(ILI9488_WHITE, ILI9488_BLACK);
    setTextSize(1);
    Button buttons[9];
    char *labels[] = { "-", "Update", "+", "Prev", "Play", "Next", "PairON", "ON", "PairOFF"};
    uint16_t xCoords[] = {50, 210, 370, 50, 210, 370, 50, 210, 370};
    uint16_t yCoords[] = {70, 70, 70, 140, 140, 140, 240, 240, 240};
    for(i = 0; i < 9; i++)
    {
        buttons[i]._xd = 0;
        buttons[i]._yd = 0;
        buttons[i]._textdatum = MC_DATUM;
        initButtonUL(&(buttons[i]), xCoords[i], yCoords[i], 80, 50,ILI9488_WHITE,
                ILI9488_BLUE,ILI9488_WHITE,*(labels + i), 2 );
        drawButton(&(buttons[i]), false);
    }
    
    fillRoundRect(20, 20, 430, 30, 5, ILI9488_PURPLE);
    sprintf(stringBuff, "Temp is : %.2f C  Press is : %.2f hPa Hum is %.3f %%RH", finalTemp, (finalPress / 100),finalHum);
    setTextColor(ILI9488_WHITE, ILI9488_PURPLE);
    setTextSize(1);
    setTextDatum(TL_DATUM);
    drawString(stringBuff, 30,30);
    while(1)
    {
//        if((millis() % 1000) < 50)
//        {
//            updateData(&finalTemp, &finalPress, &finalHum);
//            sprintf(stringBuff, "Temp is : %.2f C  Press is : %.3f hPa Hum is %.3f %%RH ", finalTemp, finalPress ,finalHum);
//            setTextColor(ILI9488_WHITE, ILI9488_PURPLE);
//            setTextSize(1);
//            setTextDatum(TL_DATUM);
//            fillRoundRect(20, 20, 430, 30, 5, ILI9488_PURPLE);
//            drawString(stringBuff, 30,30);
//        }
        bool pressed = getTouch(&xTouch, &yTouch, 4080);
        for(i = 0; i < 9; i++)
        {
            if(pressed && contains(&(buttons[i]), xTouch, yTouch))
                press(&(buttons[i]), true);
            else
                press(&(buttons[i]), false);
        }
        for(i = 0; i < 9; i++)
        {
            if(justReleased(&(buttons[i])))
                drawButton(&(buttons[i]), false);
            if(justPressed(&(buttons[i])))
            {
                drawButton(&(buttons[i]), true);
                if(i == 0)
                    BT_SendMMIAction(MMI_VOL_DOWN);
                if(i == 1)
                {
                    updateData(&finalTemp, &finalPress, &finalHum);
                    sprintf(stringBuff, "Temp is : %.2f C  Press is : %.2f hPa Hum is %.3f %%RH ", finalTemp, finalPress ,finalHum);
                    setTextColor(ILI9488_WHITE, ILI9488_PURPLE);
                    setTextSize(1);
                    setTextDatum(TL_DATUM);
                    fillRoundRect(20, 20, 430, 30, 5, ILI9488_PURPLE);
                    drawString(stringBuff, 30,30);
                }
                if(i == 2)
                    BT_SendMMIAction(MMI_VOL_UP);
                if(i == 3)
                    BT_SendMusicAction(MUSIC_PREV_SONG);
                if(i == 4)
                    BT_SendMusicAction(MUSIC_PLAY_TOGGLE);
                if(i == 5)
                    BT_SendMusicAction(MUSIC_NEXT_SONG);
                if(i == 6)
                    BT_SendMMIAction(MMI_ENTER_PAIRING_MODE);
                if(i == 7)
                    BT_Start();
                if(i == 8)
                    BT_SendMMIAction(MMI_EXIT_PAIRING_MODE);
            }
        }
//        printf("X: %d, Y: %d\n\r", xTouch, yTouch);
        delay_ms(50);
        
    }
}
//
//void __ISR(_UART_4_VECTOR, IPL2SOFT) eventHandler(void)
//{
//    uint8_t data;
//    if(INTGetFlag(INT_SOURCE_UART_RX(UART4)))
//    {
//        while(UARTReceivedDataIsAvailable(UART4))
//        {
//            circular_buf_put(cb, UART4Receive());
//        }
//        while(!circular_buf_empty(cb))
//        {
//            circular_buf_get(cb, &data);
//            if(data == 0xAA)
//                printf("\n\rReceived: ");
//            UART1HexTransmit(data);
//        }
//        //Decode CMD_ACK
//        //Calculate CHKSUM and check against received
//        INTClearFlag(INT_SOURCE_UART_RX(UART4));
//    }
//    //Clear TX flag, nothing to do here
//    if(INTGetFlag(INT_SOURCE_UART_TX(UART4)))
//    {
//        INTClearFlag(INT_SOURCE_UART_TX(UART4));
//    }
//}