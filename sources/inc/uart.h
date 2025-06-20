/*
 * uart.h
 *
 *  Created on: Jun 10, 2025
 *      Author: andre
 */

#ifndef INC_UART_H_
#define INC_UART_H_

#include <avr/io.h>

#define CPU_CLOCK		16000000UL
#define BAUDE			57600UL

void usart_init(void);
void usart_sendChar(uint8_t);
void usart_sendString(uint8_t*);

#endif /* INC_UART_H_ */
