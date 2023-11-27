/* 
 * File:   adc.c
 * Author: Pranav Ahuja
 * DESC: 
 * Created on October 30, 2023, 8:48 PM
 */

#include <xc.h>

#include "adc.h"

void init_adc()
{
    ADC_PIN_DDR = 1;
    
    ADFM = 1; 
    
    ADON = 1;               
}

unsigned short read_adc()
{
    unsigned short adc_reg_val;
    
    GO = 1;
    
    while(nDONE);
    
    adc_reg_val = (ADRESH << 8) | ADRESL;
    
    return adc_reg_val;
}
