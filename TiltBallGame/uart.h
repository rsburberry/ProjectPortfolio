#ifndef UART_H_
#define UART_H_

/*
 * Initalise the uart as some clock rate.
 */
void uart_init(uint32_t clock_rate);

/*
 * Print a charecter out over the uart.
 */
void uart_printchar(uint8_t charecter);

/*
 * Print a string out over the uart.
 */
void uart_printstring(char string[]);

/*
 * Print a number out over the uart.
 */
void uart_printnumber(uint32_t number);

#endif