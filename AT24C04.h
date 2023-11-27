/* 
 * File:   AT24C04.h
 * Author: Pranav Ahuja
 *
 * Created on November 3, 2023, 2:48 PM
 */

#ifndef AT24C04_H
#define	AT24C04_H

#include "main.h"

#define EEPROM_READ             0xA1
#define EEPROM_WRITE            0xA0

unsigned char random_read_at24c04(unsigned char address);
void write_at24c04_bytes(unsigned char address, unsigned char data);
void write_at24c04_string(unsigned char address, unsigned char *data);



#endif	/* AT24C04_H */

