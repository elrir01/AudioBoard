#include "BM64.h"
uint8_t cmd1[6] = {0xAA, 0x00, 0x02, 0x08, 0x00, 0xF6};
uint8_t cmd2[6] = {0xAA, 0x00, 0x02, 0x14, 0x18, 0xD2};
void UART_BM64_Setup(void)
{
    UARTConfigure(UART4, UART_ENABLE_PINS_TX_RX_ONLY);
    UARTSetFifoMode(UART4,UART_INTERRUPT_ON_TX_NOT_FULL | UART_INTERRUPT_ON_RX_NOT_EMPTY);
    UARTSetLineControl(UART4, UART_DATA_SIZE_8_BITS | UART_PARITY_NONE | UART_STOP_BITS_1);
    UARTSetDataRate(UART4, SYS_FREQ, 115200);
    UARTEnable(UART4, UART_ENABLE_FLAGS(UART_PERIPHERAL | UART_RX | UART_TX));
}

uint8_t UART4Receive()
{
    uint8_t data;
    while(!UARTReceivedDataIsAvailable(UART4))
        ;
    data = UARTGetDataByte(UART4);
    return data;
}

void UART4Transmit(uint8_t data )
{
    while(!UARTTransmitterIsReady(UART4))
        ;
    UARTSendDataByte(UART4, data);
    while(!UARTTransmissionHasCompleted(UART4))
        ;
}

void BT_Start()
{   
    MFB = 1;
    delay_ms(2000);
    MFB = 0;
    sendCommand(cmd1, 6);
    sendCommand(cmd2, 6);
    BT_SendMMIAction(MMI_PWR_ON_BTN_PRESS);
    delay_ms(810);
    BT_SendMMIAction(MMI_PWR_ON_BTN_RELEA);
    
}

void sendCommand(uint8_t *buff, uint8_t len)
{
    disableRX4INT();
    uint8_t i;
    printf("\n\rSending: ");
    for (i = 0; i < len; i++)
    {
        UART4Transmit(buff[i]);
        UART1HexTransmit(buff[i]);
    }
    enableRX4INT();
}
void BT_SendEventACK(uint8_t cmd)
{
    disableRX4INT();
    uint8_t i, buff[6];
    buff[0] = 0xAA;
    buff[1] = 0x00;
    buff[2] = 0x02;
    buff[3] = EVENT_ACK;
    buff[4] = cmd;
    buff[5] = calculateChecksum(&buff[2], &buff[4]);
    printf("\n\rSending: ");
    
    RX_IND = 1;

    for(i = 0; i < 6; i++)
    {
        UART4Transmit(buff[i]);
        UART1HexTransmit(buff[i]);
    }

    RX_IND = 0;
    enableRX4INT();
}

void BT_SendMMIAction(uint8_t cmd)
{
    disableRX4INT();
    uint8_t i, buff[7];
    buff[0] = 0xAA;
    buff[1] = 0x00;
    buff[2] = 0x03;
    buff[3] = MMI_CMD;
    buff[4] = 0x00; // database_index check if right
    buff[5] = cmd;
    buff[6] = calculateChecksum(&buff[2], &buff[5]);
    printf("\n\rSending: ");
    
    RX_IND = 1;

    for(i = 0; i < 7; i++)
    {
        UART4Transmit(buff[i]);
        UART1HexTransmit(buff[i]);
    }

    RX_IND = 0;
    enableRX4INT();
}

void BT_SendMusicAction(uint8_t cmd)
{
    disableRX4INT();    
    uint8_t i, buff[7];
    buff[0] = 0xAA;
    buff[1] = 0x00;
    buff[2] = 0x03;
    buff[3] = MUSIC_CTRL_CMD;
    buff[4] = 0x00; //Reserved
    buff[5] = cmd;
    buff[6] = calculateChecksum(&buff[2], &buff[5]);
    printf("\n\rSending: ");
    
    RX_IND = 1;

    for(i = 0; i < 7; i++)
    {
        UART4Transmit(buff[i]);
        UART1HexTransmit(buff[i]);
    }

    RX_IND = 0;
    enableRX4INT();
    
}
void enableRX4INT()
{
    return;
//    INTEnable(INT_SOURCE_UART_RX(UART4), INT_ENABLED);
}
void disableRX4INT()
{
    return;
//    INTEnable(INT_SOURCE_UART_RX(UART4), INT_DISABLED);
}
static uint8_t calculateChecksum(uint8_t *startByte, uint8_t *endByte)
{
    uint8_t checksum = 0;
    while(startByte <= endByte)
    {
        checksum += *startByte;
        startByte++;
    }
    checksum = ~checksum + 1;
    return checksum;
}

