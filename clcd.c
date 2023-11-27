/* 
 * File:   clcd.c
 * Author: Pranav Ahuja
 * DESC: 
 * Created on October 30, 2023, 6:38 PM
 */

#include <xc.h>
#include "clcd.h"

void init_clcd() 
{
    CLCD_PORT_DDR = 0x00;
    
    CLCD_RS_PIN_DDR = 0;
    CLCD_EN_PIN_DDR = 0;
    
    init_display_controller();
}

void init_display_controller()
{
    __delay_ms(30);
    
    clcd_write(CLCD_EIGHT_BIT_MODE, CLCD_INST_MODE);
    __delay_us(4100);
    
    clcd_write(CLCD_EIGHT_BIT_MODE, CLCD_INST_MODE);
    __delay_us(100);
    
    clcd_write(CLCD_EIGHT_BIT_MODE, CLCD_INST_MODE);
    __delay_us(1);
    
    clcd_write(CLCD_FOUR_BIT_MODE, CLCD_INST_MODE);
    __delay_us(100);
    
    clcd_write(CLCD_TWO_LINES_5X8_4_BIT_MODE, CLCD_INST_MODE);
    __delay_us(100);
    
    clcd_write(CLCD_CLEAR_DISPLAY, CLCD_INST_MODE);
    __delay_us(100);
    
    clcd_write(CLCD_DISPLAY_ON_CURSOR_OFF, CLCD_INST_MODE);
    __delay_us(100);
    
}

void clcd_write(unsigned char byte, unsigned char mode)
{
    CLCD_RS_PIN = mode;
    
    CLCD_PORT = 0xF0 & byte;
    CLCD_EN_PIN = 1;
    __delay_us(100);
    CLCD_EN_PIN = 0;
    
    CLCD_PORT = (byte & 0x0F) << 4;
    CLCD_EN_PIN = 1;
    __delay_us(100);
    CLCD_EN_PIN = 0;
    
    __delay_us(4100);
}

void clcd_putch(unsigned char data, unsigned char addr)
{
    clcd_write(addr, CLCD_INST_MODE);
    clcd_write(data, CLCD_DATA_MODE);
}

void clcd_print(unsigned char* str, unsigned char addr)
{
    clcd_write(addr, CLCD_INST_MODE);
    
    while(*str != 0)
    {
        clcd_write(*str, CLCD_DATA_MODE);
        str++;
    }
}

void clcd_specific_instruction(unsigned char instruction)
{
    __delay_us(100);
    clcd_write(instruction, CLCD_INST_MODE);
}

void clcd_goto_pos(unsigned char pos)
{
    __delay_us(100);
    clcd_write(pos, CLCD_INST_MODE);
}