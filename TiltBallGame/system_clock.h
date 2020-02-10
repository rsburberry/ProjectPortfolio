#ifndef SYSTEM_CLOCK_H_
#define SYSTEM_CLOCK_H_

#include <stdint.h>

/*
 * Initalise the system clock to the freqeuncy requested in MHz.
 */
void PLL_Init(uint32_t frequencyMhz);

#endif