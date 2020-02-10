#ifndef GPIO_H_
#define GPIO_H_

#include <stdint.h>

// Direction type for setting pin mode (input/output).
typedef enum {OUTPUT, INPUT} pinDirection;

// Function prototypes.
/** gpio initalization
 * Initalise a GPIO pin as an 'OUTPUT' or 'INPUT'.
 * The pin should be a hexadecimal encodng of the port letter
 * and pin number. For example PB3 (port B pin 3) is 0xB3.
 */
void gpio_init_pin(uint8_t pin, pinDirection dir);

/** gpio set
 * Set the pin to true/1/high/vcc.
 */
 void gpio_set(uint8_t pin);

/** gpio clear
 * Set the pin to false/0/low/vcc.
 */
void gpio_clear(uint8_t pin);

/** gpio toggle
 * Set the pin to the opposite of it's current state.
 */
void gpio_toggle(uint8_t pin);

/** gpio read
 * Return the current state on the pin. Primarily used for inputs.
 */
uint8_t gpio_read(uint8_t pin);

/** gpio set value
 * Set the pin to some value (1 or 0). The behavior of values
 * other than 0 and 1 is undefined.
 */
void gpio_set_value(uint8_t pin, uint8_t value);

#endif // GPIO_H_