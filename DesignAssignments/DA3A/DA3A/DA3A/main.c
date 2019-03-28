/*
 * DA3A.c
 *
 * Created: 3/25/2019 12:05:50 AM
 * Author : rocky
 */ 

#define F_CPU 16000000UL	// Frequency of Xplained Mini (16MHz)
#include <avr/io.h>			// Standard AVR Library
#include <stdio.h>			// AVR library containing printf functions
#include <avr/interrupt.h>	// AVR library containing interrupt functions
#include <util/delay.h>		// AVR library containing _delay_ms() function

#define BAUDRATE 9600										// Baudrate in Bits per second (bps)
#define BAUD_PRESCALLER ((F_CPU / (BAUDRATE * 16UL)) - 1)	// Baudrate Prescaler UBRR0H:UBRR0L
#define MAX_1s 15624										// 1s delay: OCR1A = (16MHz*1000ms/prescaler) -1

//Declaration of our functions
void Timer1_init(void);						// Function to initialize Timer1
void USART_init(void);						// Function to initialize USART

unsigned char USART_receive(void);			// Function to receive Serial data from UDR0
void USART_send( unsigned char data);		// Function to send individual char data into UDR0
void USART_putstring(char* StringPtr);		// Function to break string into individual chars and send

char String[] = "DA3A";						// 'String[]' is an array treated as a string when between " "
char outs[20];								// 'outs[]' used to store integer and float values into array of chars size 20
char t[10];									// 't[]' use
unsigned int rand_int;						// contains integer representing random value
float rand_float;							// contains float representing arbitrary random value	

int main(void) {
	
	TIMSK1 |= (1 << OCIE1A);			// Set Interrupt on Compare Match
	sei();								// Enable Global Interrupts
	Timer1_init();						// Call the Timer1 initialization code
	USART_init();						// Call the USART initialization code

	while (1) {												// Infinite loop
		if (TCNT1 == OCR1A) {								// Display a String when Timer1 Matches OCR1A
			rand_int = rand();								// Randomize Integer Value
			rand_float = 4.716;								// Arbitrary Float Value chosen
			USART_putstring(String);						// Pass 'String' to function to send serial of chars
			USART_send(' ');								// Puts blank space into terminal monitor
			snprintf(outs,sizeof(outs),"%3d", rand_int);	// Stores integer 'rand_int' into the string 'outs'
			USART_putstring(outs);							// Pass 'outs' to function to send serial of chars
			USART_send(' ');								// Puts blank space into terminal monitor
			dtostrf(rand_float, 3, 5, t);					// Stores float 'rand_float' as variable readable to 'snprintf'
			snprintf(outs,sizeof(outs),"%s", t);			// Stores 't' (float 'rand_float') into the string 'outs'
			USART_putstring(outs);							// Pass 'outs' to function to send serial of chars
			USART_send('\n');								// Puts new line into terminal monitor
		}
	}
	return 0;
}
//-----------------------------------------------------------------------
void Timer1_init(void) {					// Function to Initialize Timer1 properties
	OCR1A = MAX_1s;							// Let the OCR1A be the Max Value used for 1s delay
	TCCR1A = (0<<COM1A1)|(0<<COM1A0);		// Normal Operation, Disconnected OC1A 'COM1A1:0'
	TCCR1A = (0<<COM1B1)|(0<<COM1B0);		// Normal Operation, Disconnected OC1B 'COM1B1:0'
	TCCR1A = (0<<WGM11)|(0<<WGM10);			// Set Timer1 to CTC Mode 'WGM1:0'
	TCCR1B = (0<<WGM13)|(1<<WGM12);			// Set Timer1 to CTC Mode 'WGM3:2'
	TCCR1B = (1<<CS12)|(0<<CS11)|(1<<CS10);	// Set Clock Select for Prescaler '1024'
}

ISR(TIMER1_COMPA_vect) {	// Function to Truncate Timer1 to delay properly for 1 second
	TCNT1 = 0;				// Restart Timer1 from the Beginning
	return;					// Resume code from where interrupt left off
}
//-----------------------------------------------------------------------
void USART_init(void) {							// Function to Initialize USART properties
	UBRR0H = (uint8_t)(BAUD_PRESCALLER >> 8);	// Store Upper Baudrate values into UBRR0H
	UBRR0L = (uint8_t)(BAUD_PRESCALLER);		// Store Lower Baudrate values into UBRR0L
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);		// Enable Receiver and Enable Transmitter
	UCSR0C = (3 << UCSZ00);						// Set UCSZ02:1 as 8-bit character data
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

