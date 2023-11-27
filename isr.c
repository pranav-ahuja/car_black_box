/* 
 * File:   isr.c
 * Author: Pranav Ahuja
 * DESC: 
 * Created on November 5, 2023, 11:49 AM
 */

#include "main.h"

unsigned long int count = 0;
unsigned long int count_submenu_itm = 0;
unsigned long int count_time = 0; 
extern unsigned char uart_ch;
extern unsigned char screens_flag;
extern unsigned char locked;
extern unsigned char in_subscreen;
extern int time_pos;

void __interrupt() isr(void)
{
    if(RCIF == 1)
    {
        if(OERR == 1)
            OERR = 0;
        
        uart_ch = RCREG;
        RCIF = 0;       
    }
    
    if(TMR0IF == 1)
    {
        TMR0 += 8;
        if(count++ == 100000)
        {            
            count = 0;
            if(screens_flag == LOGIN_SCREEN)
            {
                clcd_specific_instruction(CLCD_CLEAR_DISPLAY);
                screens_flag = DASHBOARD_SCREEN;
                locked = 0;
            }            
        }
        else if(count_submenu_itm++ == 540000)
        {            
            count_submenu_itm = 0;
            if(screens_flag == CHANGE_PASSWORD || screens_flag == SET_TIME)
            {
                clcd_specific_instruction(CLCD_CLEAR_DISPLAY);
                screens_flag = MENU_SCREEN;            
            }            
        }                

        TMR0IF = 0;
    }
}

