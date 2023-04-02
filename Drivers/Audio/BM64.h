/* 
 * File:   BM64.h
 * Author: Mattia
 *
 * Created on 8 giugno 2022, 10.21
 */

#ifndef BM64_H
#define	BM64_H
#ifdef __XC32
    #include <xc.h>          /* Defines special funciton registers, CP0 regs  */
#endif
#define _SUPPRESS_PLIB_WARNING
#define _DISABLE_OPENADC10_CONFIGPORT_WARNING
#include <plib.h>            /* Include to use PIC32 peripheral libraries     */
#include <stdint.h>          /* For uint32_t definition                       */
#include <stdbool.h>         /* For true/false definition                     */
#include <stdio.h>
#include "../../user.h"
#include "../../system.h"

//Command ACK Status id
#define ACK_STS_OK          0
#define ACK_DISALLOWED      1
#define ACK_COMMAND_UNKNOW  2
#define ACK_PARA_ERROR      3
#define ACK_BTM_BUSY        4
#define ACK_OUT_OF_MEMORY   5

#define MMI_CMD                 0x02
#define MMI_PWR_ON_BTN_PRESS    0x51
#define MMI_PWR_ON_BTN_RELEA    0x52
#define MMI_PWR_OFF_BTN_PRESS   0x53
#define MMI_PWR_OFF_BTN_RELEA   0x54
#define MMI_ENTER_PAIRING_MODE  0x5D
#define MMI_EXIT_PAIRING_MODE   0x6B
#define MMI_VOL_UP              0x30
#define MMI_VOL_DOWN            0x31

#define MUSIC_CTRL_CMD          0x04
#define MUSIC_PLAY              0x05
#define MUSIC_PAUSE             0x06
#define MUSIC_PLAY_TOGGLE       0x07
#define MUSIC_STOP              0x08
#define MUSIC_NEXT_SONG         0x09
#define MUSIC_PREV_SONG         0x0A

#define EVENT_ACK               0x14 // acknowledge command followed by event id by the MCU

void sendCommand(uint8_t *buff, uint8_t len);
void BT_SendEventACK(uint8_t cmd);
void BT_SendMMIAction(uint8_t cmd);
void BT_SendMusicAction(uint8_t cmd);
void BT_Start();

void UART4Transmit(uint8_t data);
uint8_t UART4Receive();
void UART_BM64_Setup(void);

void enableRX4INT();
void disableRX4INT();

static uint8_t calculateChecksum(uint8_t *startByte, uint8_t *endByte);
#endif	/* BM64_H */

