/* 
 * File:   digital_keypad.h
 * Author: Pranav Ahuja
 *
 * Created on October 30, 2023, 10:34 PM
 */

#ifndef DIGITAL_KEYPAD_H
#define	DIGITAL_KEYPAD_H

#include "main.h"

#define DK_KEYPAD_PORT                      PORTB
#define DK_KEYPAD_PORTY_DDR                 TRISB

#define DK_INPUT_LINES                      0x3F
#define DK_ALL_RELEASED                     DK_INPUT_LINES

#define DK_STATE_MODE                       1
#define DK_LEVEL_MODE                       0

#define DKS1        0x3E
#define DKS2        0x3D
#define DKS3        0x3B
#define DKS4        0x37
#define DKS5        0x2F
#define DKS6        0x1F

#define LPSW4       0x11
#define LPSW5       0x22

void init_digital_keypad();
unsigned char read_digital_keypad(unsigned char mode);

#endif	/* DIGITAL_KEYPAD_H */

