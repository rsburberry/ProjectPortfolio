#ifndef TIMER_H_
#define TIMER_H_
#include <stdint.h>

// Timer 0 function prototypes.

/** Timer 0 initalization.
 * period_ms - the timer period in milliseonds
 * using_interrupts - 1 to enable interrupt generation, 0 to disable it.
 */
void timer0_init(uint32_t period_ms, uint8_t using_interrupts);

/** Timer 0 - check if finished.
 * Checks if timer 1 has completed a full period. Returns 1 if true, 0 if false.
 */
uint8_t timer0_finished();

/** Timer 0 - Clear flags.
 * Clears 'timer finished' flag. Must be called to be able to check timer1_finished
 * for the next cycle.
 */
void timer0_clear();

/** Timer 0 - Disable.
 */
void timer0_diable();

/** Timer 0 - Enable.
 */
void timer0_enable();

#endif // TIMER_H_