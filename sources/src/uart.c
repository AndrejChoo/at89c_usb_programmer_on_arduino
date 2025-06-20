/*
 * uart.c
 *
 *  Created on: Jun 10, 2025
 *      Author: andre
 */

#include "uart.h"


void usart_init(void)
{
	uint16_t divider = CPU_CLOCK/(16 * BAUDE) - 1;

	UCSR0A = 0x00;
	//UCSR0B = 0x00;
	UCSR0B = (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0);
	UCSR0C = ((1 << UCSZ00) | (1 << UCSZ01));

	UBRR0H = ((divider>>8) & 0xFF);
	UBRR0L = (divider & 0xFF);


}

void usart_sendChar(uint8_t data)
{
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = data;
}

void usart_sendString(uint8_t* str)
{
	int chr = 0;
	while(str[chr]!=0)
	{
		usart_sendChar(str[chr]);
		chr++;
	}
}

