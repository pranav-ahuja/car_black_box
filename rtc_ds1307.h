/* 
 * File:   rtc_ds1307.h
 * Author: Pranav Ahuja
 *
 * Created on October 31, 2023, 4:45 PM
 */

#ifndef RTC_DS1307_H
#define	RTC_DS1307_H

#include "main.h"

#define I2C_RTC_WRITE           0xD0
#define I2C_RTC_READ            0xD1

#define SEC_REG                 0x00
#define MIN_REG                 0x01
#define HR_REG                  0x02

void init_rtc();
unsigned char i2c_rtc_read(unsigned char address);
void i2c_rtc_write(unsigned char address, unsigned char data);
void i2c_display_time(unsigned char clock_register[3]);

#endif	/* RTC_DS1307_H */

