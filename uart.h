/* 
 * File:   uart.h
 * Author: Pranav Ahuja
 *
 * Created on November 3, 2023, 9:12 PM
 */

#ifndef UART_H
#define	UART_H

#include <xc.h>

#define UART_FOSC               20000000

void init_uart(unsigned long int baud);
void putchar(unsigned char ch);
void putstr(const char *srt);

#endif	/* UART_H */

