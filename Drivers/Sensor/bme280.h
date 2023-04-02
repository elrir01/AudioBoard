/* 
 * File:   bme280.h
 * Author: Mattia
 *
 * Created on 5 aprile 2022, 18.03
 */

#ifndef BME280_H
#define	BME280_H
#include <stdint.h>         /* For uint32_t definition                        */
#include <stdbool.h>        /* For true/false definition                      */
#include <string.h>
#include <stdio.h>
#include "../../user.h"

void BME280_init();
uint8_t BME280_read_chip_ID();
int32_t BME280_read_temp();
void BME280_read_trim_data();
int32_t BME280_read_temp();
int32_t BME280_read_press();
int32_t BME280_read_hum();
int32_t BME280_compensate_T_int32(int32_t adc_T);
uint32_t BME280_compensate_P_int64(int32_t adc_P);
uint32_t BME280_compensate_H_int32(int32_t adc_H);
void updateData(float *temp, float *press, float *hum);

#endif	/* BME280_H */

