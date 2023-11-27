/* 
 * File:   timer.c
 * Author: Pranav Ahuja
 * DESC: 
 * Created on November 5, 2023, 12:10 PM
 */

#include "main.h"
extern unsigned long int count;
extern unsigned long int count_submenu_itm;
extern unsigned long int count_time;
extern unsigned char screens_flag;

void init_timer0()
{
    T0CS = 0;
    
    PSA = 1;
    
    TMR0 = 6;
    
    PS0 = 0;
    PS1 = 0;
    PS2 = 0;
    
    TMR0IE = 1;
    TMR0IF = 0;
}

void reset_timer0()
{
    TMR0 = 6;
    count = 0;
    count_submenu_itm = 0;    
    
    TMR0IE = 1;
}
