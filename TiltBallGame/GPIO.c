#include "GPIO.h"
#include "globals.h"

// Clock Enables.
#define RCGC2          (*((volatile uint32_t *)0x400FE108))

// Register offsets.
#define DIR        0x400
#define DEN        0x51C
#define DATA       0x3FC
#define LOCK       0x520
#define CR         0x524
#define PUR        0x510

void gpio_init_pin(uint8_t pin, pinDirection dir) {
  uint32_t port_base,pin_number;
  port_base = get_port_base_from_pin(pin);
  pin_number = 0x0F & pin;
  
  // Enable the clock for the correct GPIO.
  RCGC2 |= (1U << ((pin>>4)-0xA));
  
  // Set the direciton.
  if(dir == OUTPUT) {
    // Set the pin to an output.
    REGISTER(port_base, DIR) |= (1U << pin_number);
  } else {
    // Set the pin to an input.
    REGISTER(port_base, DIR) &= ~(1U << pin_number);
    // Unlock the Commit register.
    REGISTER(port_base, LOCK) = 0x4C4F434B;
    // Set these to commit
    REGISTER(port_base, CR) |= (1U << pin_number);
    // Set the pull ups on the switches.
    REGISTER(port_base, PUR) |= (1U << pin_number);
  }
  
  // Enable the pin.
  REGISTER(port_base, DEN) |= (1U << pin_number);
}

void gpio_set(uint8_t pin) {
  REGISTER(get_port_base_from_pin(pin), DATA) |= (1U << (0x0F & pin));
}

void gpio_clear(uint8_t pin) {
  REGISTER(get_port_base_from_pin(pin), DATA) &= ~(1U << (0x0F & pin));
}

void gpio_set_value(uint8_t pin, uint8_t value) {
  if(value){
    gpio_set(pin);
  } else {
    gpio_clear(pin);
  }
}

void gpio_toggle(uint8_t pin){
  if(gpio_read(pin)){
    gpio_clear(pin);
  } else {
    gpio_set(pin);
  }
}

uint8_t gpio_read(uint8_t pin){
  return (REGISTER(get_port_base_from_pin(pin), DATA) >> (0x0F & pin)) & 1U;
}