/*
 * gpio.h
 *
 *  Created on: 10 июн. 2025 г.
 *      Author: andre
 */

#ifndef INC_GPIO_H_
#define INC_GPIO_H_

#include <avr/io.h>

#define XT1_DDR		DDRC
#define XT1_PORT	PORTC
#define XT1_PIN		5
#define BSY_DDR		DDRC
#define BSY_PORT	PINC
#define BSY_PIN		4
#define VPP_DDR		DDRD
#define VPP_PORT	PORTD
#define VPP_PIN		2
#define VDD_DDR		DDRD
#define VDD_PORT	PORTD
#define VDD_PIN		3
#define PROG_DDR	DDRD
#define PROG_PORT	PORTD
#define PROG_PIN	4
#define LATCH_DDR	DDRD
#define LATCH_PORT	PORTD
#define LATCH_PIN	5
#define MOSI_PORT	PORTC
#define MOSI_PIN	0
#define SCK_PORT	PORTC
#define SCK_PIN	1

#define XT1_HIGH	XT1_PORT |= (1 << XT1_PIN);
#define XT1_LOW		XT1_PORT &= ~(1 << XT1_PIN);

#define VPP_PROG	VPP_PORT |= (1 << VPP_PIN);
#define VPP_LOW		{VPP_PORT &= ~(1 << VPP_PIN); VDD_PORT &= ~(1 << VDD_PIN);}
#define VPP_HIGH	{VPP_PORT &= ~(1 << VPP_PIN); VDD_PORT |= (1 << VDD_PIN);}

#define PROG_HIGH	PROG_PORT |= (1 << PROG_PIN);
#define PROG_LOW	PROG_PORT &= ~(1 << PROG_PIN);

#define LATCH_HIGH	LATCH_PORT |= (1 << LATCH_PIN);
#define LATCH_LOW	LATCH_PORT &= ~(1 << LATCH_PIN);

#define MOSI_HIGH	MOSI_PORT |= (1 << MOSI_PIN);
#define MOSI_LOW	MOSI_PORT &= ~(1 << MOSI_PIN);

#define SCK_HIGH	SCK_PORT |= (1 << SCK_PIN);
#define SCK_LOW		SCK_PORT &= ~(1 << SCK_PIN);

#define READ_BSY	(BSY_PORT & (1 << BSY_PIN))

void gpio_init(void);
void data_in(void);
void data_out(void);

#endif /* INC_GPIO_H_ */
