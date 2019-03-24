/*
 * DA2C_T2_CTC_OVF_C.c
 *
 * Created: 3/19/2019 9:23:54 PM
 * Author : rocky
 */ 

#define F_CPU 16000000UL		// Used to slow delay from 16MHz to 1MHz (delay library)
#include <avr/io.h>				// Standard AVR Library
#include <avr/interrupt.h>		// AVR library containing interrupt functions
#include <util/delay.h>			// AVR library containing _delay_ms() function

#define LED 0b00000100			// Modify LED Bit (Currently: PB2)
#define SWITCH 0b00000100		// Modify SWITCH bit here (PC2 in program)
#define delay 19530				// 'delay = (16MHz*1.250s/1024) - 1' for 1250ms delay

volatile unsigned char i = 0;	// 8-bit Positive Counter 'i'

int main(void) {
	
	unsigned char d_end = delay/256;		// Quotient of Delay divided by Counter Size
	unsigned char d_leftover = delay%256;	// Remainder of Delay divided by Counter Size
	
	DDRB = LED;											// Set PB2 as an Output
	DDRC &= ~SWITCH;									// Set PC2 as an Input
	OCR0A = 0xFF;										// Load Compare Register Value
	TIMSK0 |= (1 << OCIE0A);							// Set Interrupt on Compare Match
	sei();												// Enable Global Interrupts
	TCCR0A = (0<<COM0A1)|(0<<COM0A0);					// Set Compare Output Mode
	TCCR0A = (0<<WGM01)|(0<<WGM00);						// Set WGM to Normal
	TCCR0B = (0<<WGM02)|(1<<CS02)|(0<<CS01)|(1<<CS00);	// Set WGM to Normal (Cont.), Prescaler '1024'
	
	while (1) {
		PORTB &= ~LED;						// Set Output LED PB2 to 'Low'
		PORTC |= SWITCH;					// Activate Pull-up on PC2 (resistor connected to VCC)
		if ((~PINC & SWITCH) == SWITCH){	// If SWITCH 'High' Turn LED 'ON' for 1250ms
			PORTB |= LED;					// Set Output LED PB2 to 'High'
			i = 0;							// Initialize Counter 'i' to zero
			while (i < d_end);				// Loop Counter 'i' until Delay Quotient is met
			while (TCNT0 < d_leftover);		// If Counter 'i' at end of delay, then finish remainder
			TCNT0 = 0x00;					// Restart Period and Timer0 from beginning
		}
	}
}

ISR(TIMER0_COMPA_vect) {
	i++;							// Increment Counter 'i'
	return;							// Resume code from where interrupt left off
}


