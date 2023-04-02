#include "../../user.h"
#include "bme280.h"

typedef struct
{
    uint16_t    dig_T1;
    int16_t     dig_T2;
    int16_t     dig_T3;
}BME280_T_comp;

typedef struct
{
    uint16_t    dig_P1;
    int16_t     dig_P2, dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9;
    
}BME280_P_comp;
typedef struct
{
    uint8_t     dig_H1;
    int16_t     dig_H2;
    uint8_t     dig_H3;
    int16_t     dig_H4, dig_H5;
    int8_t      dig_H6;
}BME280_H_comp;

BME280_T_comp T_data_comp;
BME280_P_comp P_data_comp;
BME280_H_comp H_data_comp;

int32_t     t_fine;
void BME280_init()
{   uint8_t i, junk;
    uint8_t param[6] = {
    0x72, // write ctrl_hum register 0xF2
    0x01, // oversampling humidity x1
    0x74, // write ctrl_meas register 0xF4
    0x27, //  oversampling pressure x1, temp x1 and forced mode 0x25 normal mode 0x27
    0x75, // config register 0xF5
    0x00 // Tstdb = 0.5 ms , Filters off, 4-wire SPI Mode
    };
    BME_CS = 0;
    for(i = 0; i < 6; i++)
    {
        SpiChnPutC(4, param[i]);
        junk = SpiChnGetC(4); // must always read, even nothing useful is returned
    }
    BME_CS = 1;
}
uint8_t BME280_read_chip_ID()
{
    uint8_t junk, data;
    BME_CS = 0;
    
    SpiChnPutC(4, 0xD0);
    junk = SpiChnGetC(4); // must always read, even nothing useful is returned
    SpiChnPutC(4, 0xFF); // dummy write
    data = SpiChnGetC(4); // get real response
    
    BME_CS = 1;
    return data;
}
void BME280_read_trim_data()
{
    uint8_t junk, i, buff[33];
    
    BME_CS = 0;
    
    SpiChnPutC(4,0x88); // first register to read, after auto-increment
    junk = SpiChnGetC(4); // must always read, even nothing useful is returned
    for(i = 0; i < 25; i++) // 25 byte read for temp, pressure and first humidity comp param
    {
        SpiChnPutC(4, 0xFF); // dummy write
        buff[i] = SpiChnGetC(4);
    }
    BME_CS = 1;
    // even numbers = LSB odd number = MSB
    T_data_comp.dig_T1 = (buff[1] << 8) | buff[0]; 
    T_data_comp.dig_T2 = (int16_t) (buff[3] << 8) | buff[2];
    T_data_comp.dig_T3 = (int16_t) (buff[5] << 8) | buff[4];
    
    P_data_comp.dig_P1 = (buff[7] << 8) | buff[6];
    P_data_comp.dig_P2 = (int16_t) (buff[9] << 8) | buff[8];
    P_data_comp.dig_P3 = (int16_t) (buff[11] << 8) | buff[10];
    P_data_comp.dig_P4 = (int16_t) (buff[13] << 8) | buff[12];
    P_data_comp.dig_P5 = (int16_t) (buff[15] << 8) | buff[14];
    P_data_comp.dig_P6 = (int16_t) (buff[17] << 8) | buff[16];
    P_data_comp.dig_P7 = (int16_t) (buff[19] << 8) | buff[18];
    P_data_comp.dig_P8 = (int16_t) (buff[21] << 8) | buff[20];
    P_data_comp.dig_P9 = (int16_t) (buff[23] << 8) | buff[22];
    

    H_data_comp.dig_H1 = buff[24];
    
    BME_CS = 0;
    SpiChnPutC(4,0xE1); // first register to read, after auto-increment
    junk = SpiChnGetC(4); // must always read, even nothing useful is returned
    for(i = 25; i < 33; i++) // last 8 byte read for humidity comp param
    {
        SpiChnPutC(4, 0xFF); // dummy write
        buff[i] = SpiChnGetC(4);
    }
    BME_CS = 1;
    H_data_comp.dig_H2 = (int16_t) (buff[26] << 8) | buff[25];
    H_data_comp.dig_H3 = buff[27];
    H_data_comp.dig_H4 = (int16_t) (buff[28] << 4) | (0x0F & buff[29]);
    H_data_comp.dig_H5 = (int16_t) (buff[31] << 4) | ( buff[30] >> 4);
    H_data_comp.dig_H6 = (int8_t) buff[32];
}


int32_t BME280_read_temp()
{
//    uint8_t temp_xlsb = 0, temp_lsb = 0, temp_msb = 0;
    uint8_t temp_lsb, temp_msb, junk;
    int32_t temp;
    
    BME_CS = 0;
    SpiChnPutC(4,0xFA); 
    junk = SpiChnGetC(4);
    
//    temp_xlsb = SPIReceive() >> 4;
    SpiChnPutC(4,0xFF); 
    temp_msb = SpiChnGetC(4); 
    
    SpiChnPutC(4,0xFF); // first register to read, after auto-increment
    temp_lsb = SpiChnGetC(4); // must always read, even nothing useful is returned
    BME_CS = 1;
//    temp = (int32_t) ((temp_msb << 12 ) | (temp_lsb << 4) | temp_xlsb);
    temp = (int32_t) ((temp_msb << 8) | temp_lsb);
    return temp;
}

int32_t BME280_read_press()
{
    uint8_t press_lsb, press_msb, junk;
    int32_t p;
    
    BME_CS = 0;
    
    SpiChnPutC(4, 0xF7);
    junk = SpiChnGetC(4);
    
    SpiChnPutC(4, 0xFF);
    press_msb = SpiChnGetC(4);
    
    SpiChnPutC(4, 0xFF);
    press_lsb = SpiChnGetC(4);
    
    BME_CS = 1;
    
    p = (int32_t) ((press_msb << 8) | press_lsb);
    return p;
}

int32_t BME280_read_hum()
{
    uint8_t hum_lsb, hum_msb, junk;
    int32_t hum;
    
    BME_CS = 0;
    
    SpiChnPutC(4, 0xFD);
    junk = SpiChnGetC(4);
    
    SpiChnPutC(4, 0xFF);
    hum_msb = SpiChnGetC(4);
    
    SpiChnPutC(4, 0xFF);
    hum_lsb = SpiChnGetC(4);
    
    BME_CS = 1;
    
    hum = (int32_t) ((hum_msb << 8) | hum_lsb);
    return hum;
}

//Returns temperature in DegC, resolution is 0.01 DegC. 
//Output value of "5123" equals 51.23 DegC
// t_fine carries fine temperature as global value
int32_t BME280_compensate_T_int32(int32_t adc_T)
{
    int32_t var1, var2, T;
    
    var1 = ((((adc_T >> 3) - ((int32_t) T_data_comp.dig_T1 << 1))) * 
            ((int32_t)T_data_comp.dig_T2)) >> 11;
    var2 = (((((adc_T >> 4) - ((int32_t) T_data_comp.dig_T1)) * ((adc_T >> 4) - 
            ((int32_t) T_data_comp.dig_T1))) >> 12) * 
            ((int32_t)T_data_comp.dig_T3)) >> 14;
    t_fine = var1 + var2;
    T = (t_fine * 5 + 128) >> 8;
    
    return T;
}

//Returns pressure in Pa as unsigned 32bit integer in Q24.8 format (24 integer bits and
// 8 fractional bits).
//Output value of "24674867" reoresebts 24674867/256 = 96386.2 Pa = 93.862 hPa
uint32_t BME280_compensate_P_int64(int32_t adc_P)
{
    int64_t var1, var2, p;
    
    var1 = ((int64_t) t_fine) - 128000;
    var2 = var1 * var1 * (int64_t) P_data_comp.dig_P6;
    var2 = var2 + ((var1 * (int64_t) P_data_comp.dig_P5) << 17);
    var2 = var2 + (((int64_t) P_data_comp.dig_P4) << 35);
    var1 = ((var1 * var1 * (int64_t) P_data_comp.dig_P3) >> 8) + 
            ((var1 * (int64_t) P_data_comp.dig_P2) << 12);
    var1 = (((((int64_t)1) << 47) + var1 )) * ((int64_t) P_data_comp.dig_P1) >> 33;
    
    if(var1 == 0)
        return 0;
    
    p = 1048576 - adc_P;
    p = (((p << 31) - var2) * 3125) / var1;
    var1 = (((int64_t) P_data_comp.dig_P9) * (p >> 13) * (p >> 13)) >> 25;
    var2 = (((int64_t) P_data_comp.dig_P8) * p) >> 19;
    p = ((p + var1 + var2) >> 8) + (((int64_t) P_data_comp.dig_P7) << 4);
    
    return (uint32_t) p;
    
}

//Returns humidity in %RH as unsigned 32 bit integer in Q22.10 format (22 integer and 10
// fractional bits).
//Output value of "47445" represents 47445/1024 = 46.333 %RH
uint32_t BME280_compensate_H_int32(int32_t adc_H)
{
    int32_t v_x1_u32r;
    
    v_x1_u32r = (t_fine - ((int32_t) 76800));
    v_x1_u32r = (((((adc_H << 14) - (((int32_t)H_data_comp.dig_H4) << 20) - 
            (((int32_t) H_data_comp.dig_H5) * v_x1_u32r)) + ((int32_t)16384)) >> 15) *
            (((((((v_x1_u32r * ((int32_t)H_data_comp.dig_H6)) >> 10) * (((v_x1_u32r *
            ((int32_t)H_data_comp.dig_H3)) >> 11) + ((int32_t)32768))) >> 10) + 
            ((int32_t)2097152)) * ((int32_t) H_data_comp.dig_H2) + 8192) >> 14));
    v_x1_u32r = (v_x1_u32r - (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7) * 
            ((int32_t) H_data_comp.dig_H1)) >> 4));
    v_x1_u32r = (v_x1_u32r < 0 ? 0 : v_x1_u32r);
    v_x1_u32r = (v_x1_u32r > 419430400 ? 419430400 : v_x1_u32r);
    
    return (uint32_t) (v_x1_u32r >> 12);
}

void updateData(float *temp, float *press, float *hum)
{
    int32_t adc_T, adc_P, adc_H, compTemp;
    uint32_t compPress, compHum;
    float finalTemp, finalPress, finalHum;
    
    adc_T = BME280_read_temp();
    compTemp = BME280_compensate_T_int32(adc_T << 4);
    adc_P = BME280_read_press();
    compPress = BME280_compensate_P_int64(adc_P << 4);
    adc_H = BME280_read_hum();
    compHum = BME280_compensate_H_int32(adc_H);
    finalTemp = ((float)compTemp) / 100;
    finalPress = ((float)compPress) / 256; 
    finalPress /= 100;
    finalHum = ((float)compHum) / 1024;
    *temp = finalTemp;
    *press = finalPress;
    *hum = finalHum;
}