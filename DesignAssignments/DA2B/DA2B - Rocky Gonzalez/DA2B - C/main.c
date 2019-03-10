/*
 * DA2B - C.c
 *
 * Created: 3/9/2019 11:04:58 AM
 * Author : rocky
 */ 

#define F_CPU 16000000		// Slow Rate of delay from 1MHz to 8MHz
#include <avr/io.h>			// Standard AVR Library
#include <avr/interrupt.h>	// AVR library containing interrupt functions
#include <util/delay.h>		// AVR library containing _delay_ms() function

#define LED 0b00000100		// Modify LED Bit here (Currently PB2)
#define ILED 0b00100000		// Modify Interrupt LED Bit here (Currently PB5)

int main(void) {			// Main Function
	DDRB = LED | ILED;		// Set LED (PB2) and ILED (PB5) as an Output
	PORTD |= 1<<2;			// Activate Pull-up in PD2
	PORTB |= LED;			// Set LED as 'High'
	PORTB &= ~ILED;			// Set ILED as 'Low'
	EICRA = 0x2;			// Set as Falling Edge Trigger
	EIMSK = (1<<INT0);		// Enable External Interrupt 0
	sei();					// Enable Global Interrupts

	while (1) {				// Loop LED Toggle, Period of 735ms (60% High, 40% Low)
		_delay_ms(435);		// Delay Program by 435ms (Multiply '16' because 16MHz)
		PORTB &= ~LED;		// Turn OFF LED (High-to-Low)
		_delay_ms(290);		// Delay Program by 290ms (Multiply '16' because 16MHz)
		PORTB |= LED;		// Turn ON LED (Low-to-High)
	}
}

ISR (INT0_vect)	{			// External Interrupt 0
	PORTB ^= (1<<5);		// Toggle PB5 LED ON and OFF
	_delay_ms(1000);		// Toggle every 1000ms
}