#ifndef INTERRUPTS_H_
#define INTERRUPTS_H_

#include <stdint.h>

/** interrupt_pin_init
 * Creates an interrupt on the provided pin.
 * The pin should be given as a hexadecimal quantity coresponding to
 * the port letter and pin number, e.g. 0xF3 is port F pin 3. (PF3).
 */
void interrupt_pin_init(uint8_t pin);

/** interrupt_pin_clear
 * Clears the flag on a specific interrupt input.
 * Pins which have been triggered should have their flags clearedThe
 * immediately after the ISR starts.
 */
void interrupt_pin_clear(uint8_t pin);

#endif // INTERRUPTS_H_