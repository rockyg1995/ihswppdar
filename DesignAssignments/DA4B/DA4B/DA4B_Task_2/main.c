/*
 * DA4B_Task_2.c
 *
 * Created: 4/19/2019 8:16:33 PM
 * Author : RYG95
 */ 

#define F_CPU 16000000UL	// Frequency of Xplained Mini (16MHz)
#include <avr/io.h>			// Standard AVR Library
#include <stdio.h>			// AVR library containing printf functions
#include <avr/interrupt.h>	// AVR library containing interrupt functions
#include <util/delay.h>		// AVR library containing _delay_ms() function

#define BAUDRATE 9600										// Baudrate (bps)
#define BAUD_PRESCALLER ((F_CPU / (BAUDRATE * 16UL)) - 1)	// Baudrate Prescaler

void Timer1_init(void);					// Initialize Timer1 properties
void adc_init(void);					// Initialize Analog to Digital Converter
void read_adc(void);					// Read value received from ADC

void USART_init(void);					// Initialize USART
unsigned char USART_receive(void);		// Receive Serial data from UDR0
void USART_send(unsigned char data);	// Send char data into UDR0
void USART_putstring(char* StringPtr);	// Break string into chars, send

volatile float adc_val;					// Stores ADC Value
char outs[20];							// Store float value into array of chars

int main(void) {
	
	DDRC = 0x00;						// PC7:0 Inputs (PC0 Read ADC, PC1 Switch)
	PORTC = (1<<DDC1);					// Set pull-up for PC1
	DDRB |= (1 << 1);					// OC1A for ATmega328PB = PB1

	Timer1_init();						// Call the Timer1/PWM initialization
	adc_init();							// Call the ADC initialization code
	USART_init();						// Call the USART initialization code
	USART_putstring("Connected!\r\n");	// Pass "Connected!" to function
	_delay_ms(125);						// Wait a bit

	while (1) {							// Infinite loop
		read_adc();						// Read ADC Value
		ICR1 = 39999;					// Set TOP count for timer1 in ICR1
		snprintf(outs,sizeof(outs),"%3f\r\n", adc_val);	// Store 'adc_val' in 'outs'
		USART_putstring(outs);			// Pass "outs" to function

		if (adc_val >= 3999) {			// If 'adc_val >= 3999'...
			TCNT1 = 0;					// Reset Timer1
			OCR1A = 3999;				// Set timer1 to position 180 degrees
			_delay_ms(100);				// delay
		} else if ((adc_val < 3999) & (adc_val >= 1999)) {	// If between range
			TCNT1 = 0;					// Reset Timer1
			OCR1A = adc_val;			// Set timer1 to position ADC based value
			_delay_ms(100);				// delay
		} else if (adc_val < 1999) {	// If 'adc_val < 1999'...
			TCNT1 = 0;					// Reset Timer1
			OCR1A = 1999;				// Set timer1 to position 0 degrees
			_delay_ms(100);				// delay
		}
	}
}
//-----------------------------------------------------------------------
void Timer1_init(void) {				// Initialize Timer0 properties
	TCCR1A = (1 << COM1A1) | (1 << WGM11);
	TCCR1B = (1 << WGM13) | (1<< WGM12) | (1 << CS11);
}
//-----------------------------------------------------------------------
void USART_init(void) {							// Initialize USART properties
	UBRR0H = (uint8_t)(BAUD_PRESCALLER >> 8);	// Store Upper Baudrate in UBRR0H
	UBRR0L = (uint8_t)(BAUD_PRESCALLER);		// Store Lower Baudrate in UBRR0L
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);		// Enable Receiver and Transmitter
	UCSR0C = (3 << UCSZ00);						// Set UCSZ02:1 as 8-bit char
}

unsigned char USART_receive(void) {				// Function to receive ASCII value from UDR0
	while (!(UCSR0A & (1 << RXC0)));			// Keep Checking until RXC0 is 'High' to break loop
	return UDR0;								// Return received serial into unsigned char data
}

void USART_send(unsigned char data) {			// Function to transmit ASCII value into UDR0
	while (!(UCSR0A & (1 << UDRE0)));			// Keep Checking until  UDRE0 data register 'High' to break loop
	UDR0 = data;								// Store unsigned char serial data into UDR0
}

void USART_putstring(char* StringPtr) {			// Function to break string into chars, then USART_send()
	while (*StringPtr != 0x00) {				// Keep Looping until String Completed (null/0-bits)
		USART_send(*StringPtr);					// Send the unsigned char value pointed by the string pointer
		StringPtr++;							// Increment pointer to next char array location
	}
}
//-----------------------------------------------------------------------
void adc_init(void) {
	DIDR0 = 0x3F;					// Disable Digital Input

	ADMUX = (0<<REFS1)|(1<<REFS0)|	// Reference Selection Bits, AVcc - External cap at AREF
	(0<<ADLAR)|						// ADC Left Adjust Result for 10-bit result
	(0<<MUX0);						// Analog Channel Selection Bits 'ADC0' (PC0)
	
	ADCSRA = (1<<ADEN)|				// ADC Enable
	(0<<ADSC)|						// ADC Start Conversion
	(0<<ADATE)|						// ADC Auto Trigger Enable
	(0<<ADIF)|						// ADC Interrupt Flag
	(0<<ADIE)|						// ADC Interrupt Enable
	(1<<ADPS2)|(1<<ADPS0);			// ADC Prescaler Select Bits '32'
}

void read_adc(void) {
	unsigned char i = 1;			// Set 'i' for iterations
	adc_val = 0;					// set float 'adc_val'
	while (i--) {					// Decrement 'i' until 4 samples take
		ADCSRA |= (1<<ADSC);		// If ADSC is high (ADC Start Conversion)...
		while (ADCSRA & (1<<ADSC));	// Start the ADC Conversion
		adc_val += ADC;				// Store the analog value on of current adc_val
		_delay_ms(50);				// delay 50ms for sampling
	}
	adc_val = (adc_val*4);			// Store  ADC value as readable
}

