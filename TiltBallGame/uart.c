#include <stdint.h>
#include "uart.h"
#include "globals.h"

// UART BASES
#define UART0_BASE 0x4000C000
#define UART1_BASE 0x4000D000
#define UART2_BASE 0x4000E000
#define UART3_BASE 0x4000F000
#define UART4_BASE 0x40010000
#define UART5_BASE 0x40011000
#define UART6_BASE 0x40012000
#define UART7_BASE 0x40013000

// UART REGISTERS
#define RCGCUART   0x400FE618
#define UARTIBRD   0x024
#define UARTFBRD   0x028
#define UARTLCRH   0x02C
#define UARTCTL    0x030
#define UARTIFLS   0x034
#define UARTIM     0x038
#define UARTMIS    0x040
#define UARTCC     0xFC8
#define UARTDR     0x000
#define UARTFR     0x018

// Clock Enables.
#define RCGC2          (*((volatile uint32_t *)0x400FE108))

// Register offsets.
#define GPIODIR        0x400
#define GPIODEN        0x51C
#define GPIODATA       0x3FC
#define GPIOLOCK       0x520
#define GPIOCR         0x524
#define GPIOPUR        0x510
#define GPIOAFSEL      0x420
#define GPIOPCTL       0x52C

// GPIO BASES
#define PORTA_BASE 0x40004000

/*  UART Initialization
 *
 */
void uart_init(uint32_t clock_rate){
  // Enable the UART0
  REGISTER(RCGCUART, 0) |= 1U;
  
  // Enable the pins for UART0 on PA0, PA1.
  RCGC2 |= 1U; // Enable Port A.
  REGISTER(PORTA_BASE, GPIOAFSEL) = 0x00000003; // Set PA0, PA1 as alternate functions.
  REGISTER(PORTA_BASE, GPIOPCTL) = 0x00000011; // Set PMC0, PMC1 to 0x1 to select UART.
  REGISTER(PORTA_BASE, GPIODEN) = 0x00000003; // Enable PA0, PA1.

  // Calculate the clock rate
  double clockDivTotal = (((double) clock_rate)*1000000.0)/1843200.0;
  uint32_t div_top = (int) clockDivTotal;
  uint32_t div_bottom = (int) ((clockDivTotal - ((double) div_top))*64.0 - 0.5);
  
  // 1. Disable the UART by clearing the UARTEN bit in the UARTCTL register.
  REGISTER(UART0_BASE, UARTCTL) &= ~(1U);
  // 2. Write the integer portion of the BRD to the UARTIBRD register.
  REGISTER(UART0_BASE, UARTIBRD) = (0xFFFF0000 & REGISTER(PORTA_BASE, UARTIBRD)) | div_top; // SET IBRD (integer)
  // 3. Write the fractional portion of the BRD to the UARTFBRD register.
  REGISTER(UART0_BASE, UARTFBRD) = (0xFFFFFF00 & REGISTER(PORTA_BASE, UARTFBRD)) | div_bottom; // SET FBRD (fractional)
  // 4. Write the desired serial parameters to the UARTLCRH register (in this case, a value of 0x00000060).
  REGISTER(UART0_BASE, UARTLCRH) = 0x00000060; // 8 data, 1 stop, no parity.
  // 5. Configure the UART clock source by writing to the UARTCC register.
  REGISTER(UART0_BASE, UARTCC) &= 0xFFFFFFF0; // System Clock.
  // 6. Optionally, configure the µDMA channel.
  //  --- SKIP ----
  // 7. Enable the UART by setting theUARTENbit in theUARTCTLregister.
  REGISTER(UART0_BASE, UARTCTL) = 0x00000301; // Enable TX and RX and UART.
}

void uart_printchar(uint8_t charecter){
  while(REGISTER(UART0_BASE, UARTFR) & (1<<5)){}
  REGISTER(UART0_BASE, UARTDR) = charecter;
}

void uart_printstring(char string[]){
  uint32_t i = 0;
  while(string[i] != '\0'){
    uart_printchar(string[i]);
    i++;
  }
}

void uart_printnumber(uint32_t number){
  uart_printchar('0' + (number/100000)%10);
  uart_printchar('0' + (number/10000)%10);
  uart_printchar('0' + (number/1000)%10);
    uart_printchar('.');
  uart_printchar('0' + (number/100)%10);
  uart_printchar('0' + (number/10)%10);
  uart_printchar('0' + (number/1)%10);
}