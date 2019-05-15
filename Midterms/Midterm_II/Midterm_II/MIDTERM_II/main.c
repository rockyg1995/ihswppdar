/*
 * MIDTERM_II.c
 *
 * Created: 5/10/2019 11:02:21 PM
 * Author : RYG95
 */ 

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>			/* Include AVR input/output file */
#include <util/delay.h>		/* Include delay functions file */
#include <math.h>			/* Include math functions file */
#include <stdlib.h>			/* Include standard library file */
#include <stdio.h>			/* Include standard input/output file */
#include "APDS9960_def.h"	/* Include MPU6050 register define file */
#include "i2c_master.h"		/* Include I2C Master header file */
#include "uart.h"			/* Include USART header file */

#define APDS9960_WRITE 0x72
#define APDS9960_READ 0x73

float C_Light;
float R_Light;
float G_Light;
float B_Light;

void init_uart(uint16_t baudrate) {

	uint16_t UBRR_val = (F_CPU/16)/(baudrate-1);

	UBRR0H = UBRR_val >> 8;
	UBRR0L = UBRR_val;

	UCSR0B |= (1<<TXEN0) | (1<<RXEN0) | (1<<RXCIE0); // UART TX (Transmit - senden) einschalten
	UCSR0C |= (1<<USBS0) | (3<<UCSZ00); //Modus Asynchron 8N1 (8 Datenbits, No Parity, 1 Stopbit)
}

void uart_putc(unsigned char c) {

	while(!(UCSR0A & (1<<UDRE0)));	// wait until sending is possible
	UDR0 = c;						// output character saved in c
}

void uart_puts(char *s) {
	while(*s) {
		uart_putc(*s);
		s++;
	}
}

void init_APDS9960(void) {
	_delay_ms(150);

	i2c_start(APDS9960_WRITE);
	i2c_write(APDS9960_ENABLE);
	i2c_write(0x00);
	i2c_stop();

	i2c_start(APDS9960_WRITE);
	i2c_write(APDS9960_ATIME);
	i2c_write(DEFAULT_ATIME);
	i2c_stop();

	i2c_start(APDS9960_WRITE);
	i2c_write(APDS9960_CONTROL);
	i2c_write(DEFAULT_AGAIN);
	i2c_stop();

	i2c_start(APDS9960_WRITE);
	i2c_write(APDS9960_ENABLE);
	i2c_write((1<<POWER)|(1<<AMBIENT_LIGHT));
	i2c_stop();

}

void getreading(void) {

	i2c_start(APDS9960_WRITE);
	i2c_write(APDS9960_CDATAH); // set pointer
	i2c_stop();
	i2c_start(APDS9960_READ);
	C_Light = (((int)i2c_read_ack()<<8) | (int)i2c_read_ack());
	i2c_stop();
	
	i2c_start(APDS9960_WRITE);
	i2c_write(APDS9960_RDATAH); // set pointer
	i2c_stop();
	i2c_start(APDS9960_READ);
	R_Light = (((int)i2c_read_ack()<<8) | (int)i2c_read_ack());
	i2c_stop();

	i2c_start(APDS9960_WRITE);
	i2c_write(APDS9960_GDATAH); // set pointer
	i2c_stop();
	i2c_start(APDS9960_READ);
	G_Light = (((int)i2c_read_ack()<<8) | (int)i2c_read_ack());
	i2c_stop();

	i2c_start(APDS9960_WRITE);
	i2c_write(APDS9960_BDATAH); // set pointer
	i2c_stop();
	i2c_start(APDS9960_READ);
	B_Light = (((int)i2c_read_ack()<<8) | (int)i2c_read_ack());
	i2c_stop();

}

int main(void) {
	char buffer[20], float_[10];
	float Cl;
	float Rl;
	float Gl;
	float Bl;

	init_uart(9600);
	i2c_init();
	init_APDS9960();

	uart_puts("TEST\r\n");

	uart_puts("AT+CWJAP=\"WIFI_NAME_HERE\",\"PASSWORD_HERE\"");	// Log in WiFi

	while(1) {
		getreading();
		Cl = C_Light;			/* Divide raw value by sensitivity scale factor to get real values */
		Rl = R_Light;
		Gl = G_Light;
		Bl = B_Light;

		dtostrf( Cl, 3, 2, float_ );	/* Take values in buffer to send all parameters over USART */
		sprintf(buffer,"%s Cl, ",float_);
		uart_puts(buffer);
		
		dtostrf( Rl, 3, 2, float_ );	/* Take values in buffer to send all parameters over USART */
		sprintf(buffer,"%s Rl, ",float_);
		uart_puts(buffer);

		dtostrf( Gl, 3, 2, float_ );	/* Take values in buffer to send all parameters over USART */
		sprintf(buffer,"%s Gl, ",float_);
		uart_puts(buffer);
		
		dtostrf( Bl, 3, 2, float_ );	/* Take values in buffer to send all parameters over USART */
		sprintf(buffer,"%s Bl, ",float_);
		uart_puts(buffer);

		uart_puts("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80");		// Connect API KEy
		uart_puts("AT+CIPSEND=51");										// Send Serial Data
		uart_puts("GET /update?key=LMPV6R4U5HWZLME7&field1=outs\r\n");	// Send Value
		uart_puts("AT+CIPCLOSE");										// Close Data

		uart_puts("\r\n");
		_delay_ms(1000);		
	}
	
	return 0;
}
