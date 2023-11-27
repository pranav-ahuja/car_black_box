/* 
 * File:   digital_keypad.c
 * Author: Pranav Ahuja
 * DESC: 
 * Created on October 30, 2023, 10:34 PM
 */

#include <xc.h>

#include "digital_keypad.h"

extern unsigned char on_hold;
void init_digital_keypad()
{
    DK_KEYPAD_PORTY_DDR = DK_KEYPAD_PORTY_DDR | DK_INPUT_LINES;
}

unsigned char read_digital_keypad(unsigned char mode)
{
    unsigned char key_here =  DK_KEYPAD_PORT & DK_INPUT_LINES ;
    static unsigned char once = 1;
    static unsigned char prekey;
    static int long_press;
    if(mode == DK_LEVEL_MODE)
    {
        if(DK_KEYPAD_PORT & DK_INPUT_LINES != DK_ALL_RELEASED)
        {
            return DK_KEYPAD_PORT & DK_INPUT_LINES;
        }
    }
    else if(mode == DK_STATE_MODE)
    {
        if(((DK_KEYPAD_PORT & DK_INPUT_LINES) != DK_ALL_RELEASED) && once)
        {
            once = 0;
            long_press = 0;
            prekey = key_here;
//            return DK_KEYPAD_PORT & DK_INPUT_LINES;
        }
        else if((!once) && (DK_KEYPAD_PORT & DK_INPUT_LINES) == DK_ALL_RELEASED)
        {
            once = 1;
            long_press = 0;
            if(long_press < 15)
                return prekey;
        }
        else if((!once) && long_press < 15)
            long_press++;
        else if((!once) && long_press == 15)
        {
            long_press = 0;
            once = 1;
            if(prekey == DKS4)
            {                             
                return LPSW4;   
            }
            else if(prekey == DKS5)
            {
                return LPSW5;
            }
            
            
        }               
    }    
    return DK_ALL_RELEASED;
}