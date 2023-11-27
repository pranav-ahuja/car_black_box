/* 
 * File:   clcd.h
 * Author: Rakesh Ahuja
 *
 * Created on October 30, 2023, 4:03 PM
 */

#ifndef CLCD_H
#define	CLCD_H

#include "main.h"

#define _XTAL_FREQ                          20000000

#define CLCD_PORT                           PORTD
#define CLCD_PORT_DDR                       TRISD

#define CLCD_RS_PIN                         RE2
#define CLCD_RS_PIN_DDR                     TRISE2

#define CLCD_EN_PIN                         RE1
#define CLCD_EN_PIN_DDR                     TRISE1

#define HI                                  1
#define LO                                  0

#define CLCD_INST_MODE                      0
#define CLCD_DATA_MODE                      1

#define CLCD_CLEAR_DISPLAY                  0x01
#define CLCD_EIGHT_BIT_MODE                 0x33
#define CLCD_FOUR_BIT_MODE                  0x02
#define CLCD_TWO_LINES_5X8_4_BIT_MODE       0x28
#define CLCD_TWO_LINES_5X8_8_BIT_MODE       0x38
#define CLCD_DISPLAY_ON_CURSOR_OFF          0x0C
#define CLCD_DISPLAY_ON_CURSOR_BLINKING     0x0F
#define CLCD_SHIFT_DISPLAY_LEFT             0x18
#define CLCD_SHIFT_DISPLAY_RIGHT            0x1C
#define CLCD_MOVE_CURSOR_LEFT               0x10
#define CLCD_MOVE_CURSOR_RIGHT              0x14
#define CLCD_POS_FOR_PASSWORD               0xC5
#define CLCD_POS_FOR_SECOND                 0xCB

#define LINE1(x)                            (0x80 + x)
#define LINE2(x)                            (0xC0 + x)

void init_clcd();
void clcd_write(unsigned char byte, unsigned char mode);
void init_display_controller();
void clcd_print(unsigned char *str, unsigned char addr);
void clcd_putch(unsigned char data, unsigned char addr);
void clcd_specific_instruction(unsigned char instruction);
void clcd_goto_pos(unsigned char pos);

#endif	/* CLCD_H */

