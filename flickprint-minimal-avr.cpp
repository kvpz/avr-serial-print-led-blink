#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#ifndef BAUD
#define BAUD 115200
#endif

#define USART_BAUD_RATE_REG_HIGH (*(volatile uint8_t*) 0xC5)
#define USART_BAUD_RATE_REG_LOW (*(volatile uint8_t*) 0xC4)
#define USART_CTRL_STATUS_REG_0B (*(volatile uint8_t *) 0xC1)
#define USART_CTRL_STATUS_REG_0C (*(volatile uint8_t *) 0xC2)

#include <stdio.h>
#include <util/delay.h>

void uart_init() {
  // Upper and lower bytes of the calculated prescaler value for baud.
  USART_BAUD_RATE_REG_HIGH = ((((F_CPU) + 8UL * (BAUD)) / (16UL * (BAUD)) -1UL) >> 8);
  
  //volatile uint8_t * regC4 = (volatile uint8_t *) 0xC4;
  USART_BAUD_RATE_REG_LOW = ((((F_CPU) + 8UL * (BAUD)) / (16UL * (BAUD)) -1UL) & 0xff);

  // Configure to enable transmitter.
  USART_CTRL_STATUS_REG_0B = (1 << 3);
  
  // Configure data frame size to 8-bits.  
  USART_CTRL_STATUS_REG_0C = ((1 << 1) | (1 << 2));
}

void uart_putchar(char c) {
  // Wait until the register to write to is free.
  volatile uint8_t * regC0 = (volatile uint8_t *) 0xC0;
  do { } while(!((*regC0) & (1 << 5)));

  // Write the byte to the USART data register.
  volatile uint8_t * regC6 = (volatile uint8_t *) 0xC6;
  *regC6 = c;
}

void uart_putstr(char *data) {
  // Loop until end of string writing char by char.
  while(*data){
    uart_putchar(*data++);
  }
}

int main()
{
  uart_init();

  // Configure port B data direction register for output
  volatile uint8_t * reg04 = (volatile uint8_t *) 0x04 + 0x20;
  *reg04 |= (1 << 5);

  volatile uint8_t * port05 = (volatile uint8_t *) 0x05 + 0x20;
  
  while(1) {
    uart_putstr("LED ON\n");
    *port05 |= (1 << 5);
    _delay_ms(1000);

    uart_putstr("LED OFF\n");
    *port05 &= ~(1 << 5);
    _delay_ms(1000); 
  }

  return 0;
}
