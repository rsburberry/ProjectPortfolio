#include "adc.h"
#include "globals.h"

// Globals
volatile double temp_reading = 0;
volatile int temp_reading_int = 0;

// ADC Registers
#define RCGCADC  (*((volatile uint32_t *)0x400FE638))
#define ADC0_BASE                          0x40038000
#define ADC1_BASE                          0x40039000
#define ADCACTSS                                0x000
#define ADCEMUX                                 0x014
#define ADCSSMUX3                               0x0A0
#define ADCSSCTL3                               0x0A4
#define ADCSSFIFO3                              0x0A8
#define ADCISC                                  0x00C
#define ADCIM                                   0x008
#define ADCRIS                                  0x004
#define NVIC_EN0 (*((volatile uint32_t *)0xE000E100))

void ADC0_Init(){
  RCGCADC |= 1; // enable ADC0
  for(uint32_t i = 0; i < 10000; i++) { asm ("nop"); } // Small delay to ensure ADC0 enable took.
  REGISTER(ADC0_BASE, ADCACTSS) &= ~0x0000000F; // Disable the sample sequencers.
  REGISTER(ADC0_BASE, ADCEMUX) |= 0x00005000; // Set the Sequencer 3 to be triggered the timer,
  REGISTER(ADC0_BASE, ADCSSMUX3) = 0; // AIN0 (PE3) should be our analog input.
  REGISTER(ADC0_BASE, ADCSSCTL3) |= 0xE; // END | IE | TS
  REGISTER(ADC0_BASE, ADCIM) |= (1U << 3); // Interrupt mask for PE3
  NVIC_EN0 |= 1 << 17; // enable the interrupt of sequencer 3.
  REGISTER(ADC0_BASE, ADCACTSS) = 1<<3; // Enable sample sequencer.
}

void ADC0_Handler(){
  // Make sure that the conversion is finished.
  while(REGISTER(ADC0_BASE, ADCRIS) & 0x8 == 0) { }

  // Calculate the temperature from ADC0_FIFO3
  temp_reading = 1000*(147.5 - ((((double) REGISTER(ADC0_BASE, ADCSSFIFO3)) * 247.5) / 4096.0));
  temp_reading_int = (int) temp_reading;
  
  // Clear the ISR.
  REGISTER(ADC0_BASE, ADCISC) = 0x8;
}

uint32_t getCPUTemp(){
  return temp_reading_int;
}

double getCPUTemp_accurate(){
  return temp_reading;
}