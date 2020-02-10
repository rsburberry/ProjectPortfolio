#include "system_clock.h"
#include "globals.h"

// System Clock control registers.
#define RCC            (*((volatile uint32_t *)0x400FE060))
#define RCC2           (*((volatile uint32_t *)0x400FE070))
#define CLOCK_RIS      (*((volatile uint32_t *)0x400FE050))

// RCC2 Regions.
#define RCC2_RESERVED  0x203F978F
#define RCC2_USERCC2   (1U << 31)
#define RCC2_DIV400    (1U << 30)
#define RCC2_PWEDN2    (1U << 11)
#define RCC2_SYSDIV2_OFFSET   22U

/**
 * PLL_Init takes an input frequency in MHz and sets the micrcontroller
 * main clock to that frequency.
 */
void PLL_Init(uint32_t frequencyMhz) {
  // Calculate the clock divider.
  uint32_t clock_div = (400/frequencyMhz) - 1;
  
  // Set RCC before we set RCC2.
  RCC &= ~(0x7F << 4); // Clear OSCSRC and XTAL bits.
  RCC |= (0x16 << 6);  // Set the XTAL.  0x15 is the 16mHz USB crystal... 0x16 is the 16.384 MHz crystal.
  
  // Set up the timer on RCC2.
  RCC2 &= RCC2_RESERVED; // Clear all non-reserved ports.
  RCC2 |= RCC2_USERCC2 | RCC2_DIV400 | RCC2_PWEDN2;
  RCC2 |= (clock_div << RCC2_SYSDIV2_OFFSET);
  while (!(CLOCK_RIS & 0x40)) {}; // Wait for the PLL to stabilize.
  RCC2 &= ~RCC2_PWEDN2; // Turn of RCC2 Override.
}