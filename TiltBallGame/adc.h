#ifndef ADC_H_
#define ADC_H_

#include <stdint.h>

/*
 * Initialise the ADC0
 */
void ADC_Init();

/*
 * ISR Handler for when the ADC conversions finish.
 */
void ADC0_Handler();

/*
 * Get the last CPU temps in thousandths of a degree C.
 */
uint32_t getCPUTemp();

/*
 * Get the last CPU temp in degrees C, as a double.
 */
double getCPUTemp_accurate();

#endif