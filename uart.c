/* 
 * File:   uart.c
 * Author: Pranav Ahuja
 * DESC: 
 * Created on November 3, 2023, 9:12 PM
 */

#include "main.h"

void init_uart(unsigned long int baud)
{
    //TRANSMITTER
    
    BRGH = 1;
    
    TXEN = 1;
    
    SPEN = 1;
    
    //RECEIVER
    
    CREN = 1;
    
    RCIE = 1;
    
    //Baud Rate
      
    SPBRG = (UART_FOSC / (16 * baud) - 1);        
    
}

void putchar(unsigned char ch)
{
    TXREG = ch;
    
    while(TXIF != 1);

    TXIF = 0;
}

void putstr(const char *str)
{
    while(*str)
    {
        putchar(*str++);
    }
}
