#include "timer.h"
#include "gpio.h"
#include "globals.h"

// Timer registers:
#define RCGCTIMER         (*((volatile uint32_t *)0x400FE604))
#define TIMER0_GPTMCTL    (*((volatile uint32_t *)0x4003000C))
#define TIMER0_GPTMTAILR  (*((volatile uint32_t *)0x40030028))
#define TIMER0_GPTMCFG    (*((volatile uint32_t *)0x40030000))
#define TIMER0_GPTMIMR    (*((volatile uint32_t *)0x40030018))
#define TIMER0_GPTMRIS    (*((volatile uint32_t *)0x4003001C))
#define TIMER0_GPTMTAMR   (*((volatile uint32_t *)0x40030004))
#define TIMER0_GPTMIMR    (*((volatile uint32_t *)0x40030018))
#define TIMER0_GPTMICR    (*((volatile uint32_t *)0x40030024))
#define NVIC_EN0          (*((volatile uint32_t *)0xE000E100))

void timer0_init(uint32_t period_ms, uint8_t using_interrupts) {
  RCGCTIMER |= 1U; // RCGCTIMER enables the timer
  TIMER0_GPTMCTL = 0; // GPTMCTL timer en (Disable timer during config.)
  TIMER0_GPTMCFG = 0; // Set to be a 32-bit timer for the wide timer (32/64-bit)
  TIMER0_GPTMTAMR = 2; // GPTMTAMR timer mode: Periodic.
  TIMER0_GPTMTAILR = 14000*period_ms; // GPTMTAILR load register, sets period.
  TIMER0_GPTMCTL |= 1U << 5; // Set the adc output. // ONLY FOR LAB3, starts the ADC conversion.
  TIMER0_GPTMCTL |= 1U; //GPTMCTL timer en (enable timer 1A).
  if(using_interrupts) {
    TIMER0_GPTMIMR = (1U);   // GPTMIMR set interrupt on timeout.
    NVIC_EN0 |= (1 << 19); // Enable interrupt 19, for TIMER0A
  }
}

void timer0_diable(){
  TIMER0_GPTMCTL = 0; // Disable timer 1.
}

void timer0_enable(){
  TIMER0_GPTMCTL |= 1U; // Enable timer 1;
}

uint8_t timer0_finished() {
  return TIMER0_GPTMRIS & 0x1; 
}

void timer0_clear() {
  TIMER0_GPTMICR |= 0x1;
}
