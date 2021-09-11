#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#ifndef BAUD
#define BAUD 9600
#endif

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
//#include <util/setbaud.h>

void uart_init() {
  // Upper and lower bytes of the calculated prescaler value for baud.
  //UBRR0H = UBRRH_VALUE;
  // Next lines of code are similar to the one above & reduces hex by 4 bytes... why?
  //_SFR_MEM8(0xC5) = UBRRH_VALUE;
  //_MMIO_BYTE(0xC5) = UBRRH_VALUE;
  volatile uint8_t * reg = (volatile uint8_t*) 0xC5;
  reg = ((((F_CPU) + 8UL * (BAUD)) / (16UL * (BAUD)) -1UL) >> 8);
  
  //UBRR0L = UBRRL_VALUE;
  volatile uint8_t * regC4 = (volatile uint8_t *) 0xC4;
  *regC4 = (volatile uint8_t)((((F_CPU) + 8UL * (BAUD)) / (16UL * (BAUD)) -1UL) & 0xff); // (UBRR_VALUE & 0xff);
  
  // Configure data frame size to 8-bits.
  //UCSR0C = _BV(UCSZ01) | _BV(UCSZ00);
  volatile uint8_t * regC2 = (volatile uint8_t *) 0xC2;
  *regC2 = ((1 << 1) | (1 << 2));
  
  // Configure to enable transmitter.
  //UCSR0B = _BV(TXEN0);
  volatile uint8_t * regC1 = (volatile uint8_t *) 0xC1;
  *regC1 = (1 << 3);
}

void uart_putchar(char c) {
  // Wait until the register to write to is free.
  // USRE0 = 5 defined in /avr/iom328p.h
  // UCSR0A = _SFR_MEM8(0xC0) = _MMIO_BYTE(mem_addr) = *(volatile uint8_t *)(mem_addr) defined in iom328p.h & sfr_defs.h
  // Function is defined in /avr/sfr_defs.h
  //loop_until_bit_is_set(UCSR0A, UDRE0);
  volatile uint8_t * regC0 = (volatile uint8_t *) 0xC0;
  do { } while(!((*regC0) & (1 << 5)));

  // Write the byte to the USART data register.
  // USR0 is defined in /avr/sfr_defs.h
  //UDR0 = c;
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

  // Configure port B data direction as output
  DDRB |= _BV(DDB5);
  
  while(1) {
    uart_putstr("LED ON\n");
    PORTB |= (1 << PORTB5);
    _delay_ms(1000);

    uart_putstr("LED OFF\n");
    PORTB &= ~_BV(PORTB5);
    _delay_ms(1000); 
  }

  return 0;
}
