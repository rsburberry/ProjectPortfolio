#include "globals.h"

// Port bases.
#define PORTA_BASE 0x40004000
#define PORTB_BASE 0x40005000
#define PORTC_BASE 0x40006000
#define PORTD_BASE 0x40007000
#define PORTE_BASE 0x40024000
#define PORTF_BASE 0x40025000

uint32_t get_port_base_from_pin(uint8_t pin) {
  switch(pin&0xF0){
    case 0xA0:
      return PORTA_BASE;
      break;
    case 0xB0:
      return PORTB_BASE;
      break;
    case 0xC0:
      return PORTC_BASE;
      break;
    case 0xD0:
      return PORTD_BASE;
      break;
    case 0xE0:
      return PORTE_BASE;
      break;
    case 0xF0:
      return PORTF_BASE;
      break;
  default:
      UsageFault_Handler();
      return 0x00;
    break;
  }
}