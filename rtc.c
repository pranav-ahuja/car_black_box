/*
 * File:   rtc.c
 * Author: Rakesh Ahuja
 *
 * Created on October 31, 2023, 3:35 PM
 */


#include "i2c.h"
#include "rtc_ds1307.h"

void init_rtc()
{
    unsigned char dummy;
    
    dummy = i2c_rtc_read(SEC_REG);
    dummy &= 0x7F;  //Initiate
    i2c_rtc_write(SEC_REG, dummy);
}

unsigned char i2c_rtc_read(unsigned char address)
{
    unsigned char data;
    
    i2c_wait_for_idle(); 
    i2c_start();
    i2c_write(I2C_RTC_WRITE);
    i2c_write(address);
    
    i2c_repeat_start();
    i2c_write(I2C_RTC_READ);
    data = i2c_read(1);
    
    i2c_stop();
    
    return data;
}

void i2c_rtc_write(unsigned char address, unsigned char data)
{
    i2c_wait_for_idle();
    i2c_start();
    i2c_write(I2C_RTC_WRITE);
    i2c_write(address);
    i2c_write(data);
    i2c_stop();
}