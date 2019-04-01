/*
 * DA3B.c
 *
 * Created: 3/29/2019 6:58:23 PM
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
void adc_init(void);						// Function to initialize Analog to Digital Converter

unsigned char USART_receive(void);			// Function to receive Serial data from UDR0
void USART_send(unsigned char data);		// Function to send individual char data into UDR0
void USART_putstring(char* StringPtr);		// Function to break string into individual chars and send
void read_adc(void);						// Function to read temperature received from ADC

volatile float adc_temp;					// Stores ADC Value representing Temperature
char outs[20];								// 'outs[]' used to store integer and float values into array of chars size 20

int main(void) {
	
	
	TIMSK1 |= (1 << OCIE1A);			// Set Interrupt on Compare Match
	sei();								// Enable Global Interrupts
	Timer1_init();						// Call the Timer1 initialization code
	USART_init();						// Call the USART initialization code
	adc_init();							// Call the ADC initialization code
	USART_putstring("Connected!\r\n");	// Pass 'Connected!' to function to send serial of chars
	_delay_ms(125);						// Wait a bit
	float adc_tempf;					// to store ADC Fahrenheit Temperature

	while (1) {												// Infinite loop
		if (TCNT1 == OCR1A) {								// Display a String when Timer1 Matches OCR1A
			read_adc();										// Read value of ADC Temperature
			adc_tempf = (ADCH << 8) + ADCL;					// Let 'T(C) = Vout/10mV' and 'TOS = ADC - T(C)'
			adc_tempf = (9/5)*adc_tempf + 32;				// Converts Celsius to Fahrenheit
			snprintf(outs,sizeof(outs),"%3f\r\n", adc_tempf);// Stores integer 'adc_temp' into the string 'outs'
			USART_putstring(outs);							// Pass 'outs' to function to send serial of chars
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
void adc_init(void) {
	ADMUX = (0<<REFS1)|(1<<REFS0)|			// Reference Selection Bits, AVcc - External cap at AREF
	(0<<ADLAR)|								// ADC Left Adjust Result
	(0<<MUX3)|(1<<MUX2)|(0<<MUX1)|(1<<MUX0);// Analog Channel Selection Bits 'ADC5' (PC5)
	
	ADCSRA = (1<<ADEN)|						// ADC Enable
	(0<<ADSC)|								// ADC Start Conversion
	(0<<ADATE)|								// ADC Auto Trigger Enable
	(0<<ADIF)|								// ADC Interrupt Flag
	(0<<ADIE)|								// ADC Interrupt Enable
	(1<<ADPS2)|(0<<ADPS1)|(1<<ADPS0);		// ADC Prescaler Select Bits '32'
}

void read_adc(void) {
	unsigned char i = 4;				// Set 'i' for iterations
	adc_temp = 0;						// set float 'adc_temp'
	while (i--) {						// Decrement 'i' until 4 samples take
		ADCSRA |= (1<<ADSC);			// If ADSC is high (ADC Start Conversion)...
		while (ADCSRA & (1<<ADSC));		// Start the ADC Conversion
		adc_temp += ADC;				// Store the analog value on of current adc_temp
		_delay_ms(50);					// delay 50ms for sampling
	}
	adc_temp = (adc_temp/4);			// Average of 4 samples taken into adc_temp
}
