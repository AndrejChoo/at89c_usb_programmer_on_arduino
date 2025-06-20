/*
 * main.c
 *
 *  Created on: Jun 10, 2025
 *      Author: andre
 */


#include <avr/io.h>
#include <avr/interrupt.h>
#include "uart.h"
#include "gpio.h"
#include "progger.h"


uint8_t exchange[530];
volatile uint16_t rx_counter = 0;
volatile uint8_t full_buff = 0;
volatile uint16_t packet_size = 0;
uint8_t main_buff[530];
volatile uint8_t nxt = 0;

ISR(USART_RX_vect)
{
	if(!(UCSR0A & ((1 << FE0) | (1 << DOR0) | (1 << UPE0))))
	{
		exchange[rx_counter] = UDR0;
		rx_counter++;
		if(rx_counter == 4) {packet_size = (exchange[1] << 8) | (exchange[2]);}
		if((packet_size > 0) && (rx_counter == packet_size + 4))
		{
			rx_counter = 0;
			packet_size = 0;
			if(exchange[3] == 0x3D) nxt = 1;
			full_buff = 1;
		}
	}
}

int main(void)
{
	usart_init();
	gpio_init();

	sei();

	while(1)
	{
		if(full_buff)
		{
			deassemble_frame();
			full_buff = 0;
		}
		asm(" ");
	}
}

