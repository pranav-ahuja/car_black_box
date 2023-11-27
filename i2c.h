/* 
 * File:   i2c.h
 * Author: Pranav Ahuja
 *
 * Created on October 31, 2023, 3:40 PM
 */

#ifndef I2C_H
#define	I2C_H

#include "main.h"

#define I2C_XTAL_FREQ           20000000

void i2c_init(unsigned long int baud);
void i2c_wait_for_idle();

void i2c_start();
void i2c_stop();
void i2c_repeat_start();

unsigned char i2c_write(unsigned char data);
unsigned char i2c_read(unsigned char ack);

#endif	/* I2C_H */

