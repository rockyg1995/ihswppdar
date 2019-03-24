/*
 * DA2C_T1_Normal_C.c
 *
 * Created: 3/12/2019 11:25:45 PM
 * Author : rocky
 */

#define F_CPU 16000000UL	// Used to slow delay from 16MHz to 1MHz (delay library)
#include <avr/io.h>			// Standard AVR Library
#include <util/delay.h>		// AVR library containing _delay_ms() function

#define LED 0b00000100		// Modify LED Bit (Currently: PB2)
#define period 11327		// 'period = (16MHz*0.725s/1024) - 1' for 735ms period
#define duty_cycle 6796		// 'duty_cycle = (16MHz*0.435s/1024) - 1' for 60% Duty Cycle

int main(void) {
	
	unsigned char p_end = period/256;			// Quotient of Period divided by Counter Size
	unsigned char p_leftover = period%256;		// Remainder of Period divided by Counter Size
	unsigned char dc_end = duty_cycle/256;		// Quotient of Duty Cycle divided by Counter Size
	unsigned char dc_leftover = duty_cycle%256;	// Remainder of Duty Cycle divided by Counter Size
	unsigned char i = 0;						// 8-bit Positive Counter 'i'
	
	DDRB = LED;											// Set direction of LED (PB2) as an Output
	TCCR0A = (0<<WGM01)|(0<<WGM00);						// Set WGM to Normal
	TCCR0B = (0<<WGM02)|(1<<CS02)|(0<<CS01)|(1<<CS00);	// Set WGM to Normal (Cont.), Prescaler '1024'
	
	while (1) {
		PORTB |= LED;						// Set Output LED PB2 to 'HIGH'
		i = 0;								// Initialize Counter 'i' to zero
		while (i < p_end) {					// Loop Counter 'i' until Period Quotient is met
			if (i == dc_end) {				// If Counter 'i' is at end of Duty Cycle Quotient...
				while(TCNT0 < dc_leftover);	// Finish Duty Cycle Remainder
				PORTB = 0x00;				// Then set Output LED PB2 to 'LOW'
			}
			while((TIFR0 & 0x01) == 0);		// Otherwise, Check if Timer0 Overflow Set
			TCNT0 = 0x00;					// If Overflow set, Restart Timer0
			TIFR0 = 0x01;					// And Reset Overflow Flag
			i++;							// Increment Counter 'i'
		}
		while (TCNT0 < p_leftover);			// If Counter 'i' at end of period, then finish remainder
		TCNT0 = 0x00;						// Restart Period and Timer0 from beginning
	}
}
