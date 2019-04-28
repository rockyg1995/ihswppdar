/*
 * DA5.c
 *
 * Created: 4/27/2019 7:16:05 AM
 * Author : RYG95
 */ 

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

//	Set up UART for printf();
#ifndef BAUD
#define BAUD 9600
#endif
#include "STDIO_UART.h"

//	Include nRF24L01+ library
#include "nrf24l01.h"
#include "nrf24l01-mnemonics.h"
#include "spi.h"
void print_config(void);

void adc_init(void);			// Function to initialize Analog to Digital Converter
void read_adc(void);			// Function to read temperature received from ADC
void USART_send(unsigned char data);		// Function to send individual char data into UDR0
void USART_putstring(char* StringPtr);		// Function to break string into individual chars and send

volatile float adc_temp;		// Stores ADC Value representing Temperature
char outs[20];					// 'outs[]' used to store integer and float values into array of chars size 20

//	Used in IRQ ISR
volatile bool message_received = false;
volatile bool status = false;

int main(void) {
	
	//	Set cliche message to send (message cannot exceed 32 characters)
	char tx_message[32];				// Define string array
	strcpy(tx_message,"Hello World!");	// Copy string into array
	
	//	Initialize UART
	uart_init();
	adc_init();
	float adc_tempf;

	//	Initialize nRF24L01+ and print configuration info
	nrf24_init();
	print_config();
	
	//	Start listening to incoming messages
	nrf24_start_listening();
	status = nrf24_send_message(tx_message);
	if (status == true) {
		printf("Message sent successfully\n")
		read_adc();
		adc_tempf = (ADCH << 8) + ADCL;					// Let 'T(C) = Vout/10mV' and 'TOS = ADC - T(C)'
		adc_tempf = (9/5)*adc_tempf + 32;				// Converts Celsius to Fahrenheit
		snprintf(outs,sizeof(outs),"%3f\r\n", adc_tempf);// Stores integer 'adc_temp' into the string 'outs'
		USART_putstring(outs);
	}
	
	while (1) {
		
		status = nrf24_send_message(tx_message);
		
		if (message_received) {
			
			//	Message received, print it
			message_received = false;
			printf("Received message from geo: %s\n",nrf24_read_message());
			//	Send message as response
			_delay_ms(500);
			if (status == true) printf("Message sent successfully\n");
		}
	}
}

//	Interrupt on IRQ pin
ISR(INT0_vect) {
	message_received = true;
}

void print_config(void) {
	uint8_t data;
	printf("Startup successful\n\n nRF24L01+ configured as:\n");
	printf("-------------------------------------------\n");
	nrf24_read(CONFIG,&data,1);
	printf("CONFIG		0x%x\n",data);
	nrf24_read(EN_AA,&data,1);
	printf("EN_AA			0x%x\n",data);
	nrf24_read(EN_RXADDR,&data,1);
	printf("EN_RXADDR		0x%x\n",data);
	nrf24_read(SETUP_RETR,&data,1);
	printf("SETUP_RETR		0x%x\n",data);
	nrf24_read(RF_CH,&data,1);
	printf("RF_CH			0x%x\n",data);
	nrf24_read(RF_SETUP,&data,1);
	printf("RF_SETUP		0x%x\n",data);
	nrf24_read(STATUS,&data,1);
	printf("STATUS		0x%x\n",data);
	nrf24_read(FEATURE,&data,1);
	printf("FEATURE		0x%x\n",data);
	printf("-------------------------------------------\n\n");
}

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