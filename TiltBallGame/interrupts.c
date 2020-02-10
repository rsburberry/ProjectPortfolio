#include "interrupts.h"
#include "globals.h"

#define NVIC_EN0 (*((volatile uint32_t *)0xE000E100))
#define IS 0x404 // Interrupt sense register offset, 0 for edge, 1 for level.
#define IM 0x410 // Interrupt mask register offset.
#define IC 0x41C // Interrupt clear register offset.

void interrupt_pin_init(uint8_t pin) {
  // Calculate the port base and pin number from the pin.
  uint32_t port_base, pin_number;
  port_base = get_port_base_from_pin(pin);
  pin_number = 0x0F & pin;
  
  // Enable interrupt for the appropriate port.
  // GPIO A is interrupt number 0, ... , E is interrupt number 4 
  // GPIO F is interrupt number 30.
  NVIC_EN0 |= ((pin & 0xF0) != 0xF0) ? (1U << ((pin>>4)-0xA)) : (1U << 30);

  // Set edge sense.
  REGISTER(port_base, IS) &= ~(1U << pin_number);

  // Set the interrupt mask for the specific pin.
  REGISTER(port_base, IM) |= (1U << pin_number);
}

void interrupt_pin_clear(uint8_t pin) {
  REGISTER(get_port_base_from_pin(pin), IC) |= (1U << (0x0F & pin));
}

// ########################################################################## //
// Interrupt handlers ####################################################### //
// ########################################################################## //

void ISR_GPIOA (void) {
  // STUB for interrupts on GPOIOA
}
void ISR_GPIOB (void) {
  // STUB for interrupts on GPOIOB
}
void ISR_GPIOC (void) {
  // STUB for interrupts on GPOIOC
}
void ISR_GPIOD (void) {
  // STUB for interrupts on GPOIOD  
}
void ISR_GPIOE(void) {
  // STUB for interrupts on GPOIOF
}