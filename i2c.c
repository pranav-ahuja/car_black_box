/* 
 * File:   i2c.c
 * Author: Pranav Ahuja
 * DESC: 
 * Created on October 31, 2023, 3:40 PM
 */

#include "i2c.h"

void i2c_init(unsigned long int baud)
{
    SSPEN = 1;
    
    SSPM3 = 1;
    
    SSPADD = (I2C_XTAL_FREQ / (baud * 4)) - 1; 
}

void i2c_wait_for_idle()
{
    while(R_nW || (SSPCON2 & 0x1F));
}

void i2c_start()
{
    i2c_wait_for_idle();
    SEN = 1;    
}

void i2c_stop()
{
    i2c_wait_for_idle();
    PEN = 1;
}

void i2c_repeat_start()
{
    i2c_wait_for_idle();
    RSEN = 1;
}

unsigned char i2c_write(unsigned char data)
{
    i2c_wait_for_idle();
    
    SSPBUF = data;
    
    return !ACKSTAT;
}

unsigned char i2c_read(unsigned char ack)
{
    unsigned char data;
    
    i2c_wait_for_idle();
    RCEN = 1;
    
    i2c_wait_for_idle();
    data = SSPBUF;
    
    if(ack == 0)
    {
        ACKDT = 0;
    }
    else if(ack == 1)
    {
        ACKDT = 1;
    }
    ACKEN = 1;
    
    return data;
}
