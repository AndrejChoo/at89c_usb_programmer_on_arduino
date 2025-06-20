/*
 * gpio.c
 *
 *  Created on: 10 июн. 2025 г.
 *      Author: andre
 */

#include "gpio.h"

void gpio_init(void)
{
	//DATA pins {PD[7:6],PB[5:0]} IN
	DDRB = 0x00;
	DDRD &= ~(3 << 6);

	//COMMAND Pins P37,P35,P34,P33 - PC[3:0]
	DDRC |= 0x0F;
	PORTB &= 0xF0;

	//XTAIL Pin
	XT1_DDR |= (1 << XT1_PIN);
	XT1_LOW

	//VPP Pin
	VPP_DDR |= (1 << VPP_PIN);
	VPP_PORT &= ~(1 << VPP_PIN);

	//VDD Pin
	VDD_DDR |= (1 << VDD_PIN);
	VDD_PORT |= (1 << VDD_PIN);

	//PROG Pin
	PROG_DDR |= (1 << PROG_PIN);
	PROG_PORT |= (1 << PROG_PIN);

	//LATCH Pin
	LATCH_DDR |= (1 << LATCH_PIN);
	LATCH_PORT |= (1 << LATCH_PIN);
	LATCH_LOW

	//BSY Pin
	BSY_DDR &= ~(1 << BSY_PIN);

}

void data_in(void)
{
	DDRB = 0x00;
	DDRD &= ~(3 << 6);
}

void data_out(void)
{
	DDRB = 0x3F;
	DDRD |= (3 << 6);
}
