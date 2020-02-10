#ifndef GLOBALS_H_
#define GLOBALS_H_

#include <stdint.h>

// Onboard (on the tiva PCB) connecitons
#define PCB_RED_LED    0xF1
#define PCB_GREEN_LED  0xF3
#define PCB_BLUE_LED   0xF2
#define PCB_SW_1       0xF0
#define PCB_SW_2       0xF4

// Pin settings for offboard (breadboard) hardware
#define RED_LED    0xE5
#define GREEN_LED  0xE3
#define YELLOW_LED 0xE4
#define SW_1       0xE1
#define SW_2       0xE2


// Specific Register access.
#define REGISTER(base, offset) (*((volatile uint32_t *) (base + offset)))

// Prototypes.
/** get port base from pin
 * Returns the port base for this pin. 
 */
uint32_t get_port_base_from_pin(uint8_t pin);

#endif