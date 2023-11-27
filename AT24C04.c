/* 
 * File:   AT24C04.c
 * Author: Pranav Ahuja
 * DESC: 
 * Created on November 3, 2023, 3:24 PM
 */

#include <xc.h>

#include "i2c.h"
#include "AT24C04.h"

unsigned char random_read_at24c04(unsigned char address)
{
    unsigned char data;
    
    i2c_start();
    i2c_write(EEPROM_WRITE);
    i2c_write(address);
    
    i2c_start();   
    i2c_write(EEPROM_READ);
    data = i2c_read(0);    
    i2c_stop();
    
    return data;
    
}

void write_at24c04_bytes(unsigned char address, unsigned char data)
{
    i2c_start();
    i2c_write(EEPROM_WRITE);
    i2c_write(address);
    i2c_write(data);
    i2c_stop();
}

void write_at24c04_string(unsigned char address, unsigned char *data)
{        
    while(*data != '\0')
    {
        write_at24c04_bytes(address, *data);
        address++;
        data++;
    }
    
    i2c_stop();
    
}