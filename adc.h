/* 
 * File:   adc.h
 * Author: Rakesh Ahuja
 *
 * Created on October 30, 2023, 8:38 PM
 */

#ifndef ADC_H
#define	ADC_H

#include "main.h"

#define ADC_PIN                     RA0
#define ADC_PIN_DDR                 TRISA0

void init_adc();
unsigned short read_adc();
#endif	/* ADC_H */

