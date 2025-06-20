/*
 * progger.h
 *
 *  Created on: 10 июн. 2025 г.
 *      Author: andre
 */

#ifndef INC_PROGGER_H_
#define INC_PROGGER_H_

#include <avr/io.h>
#include <util/delay.h>
#include "uart.h"
#include "gpio.h"


//COMMANDS
#define	WRCODE		0b1110
#define	RDCODE		0b1100
#define	WRLOCK1		0b1111
#define	WRLOCK2		0b0011
#define	WRLOCK3		0b0101
#define	ERASE		0b0001
#define	RDSIGN		0b0000

//
void deassemble_frame(void);//������� ������� �������
void send_successConn(void); //������� �������� ������ "������������ ������������"
void send_frame(uint8_t *buff, uint16_t count); //������� �������� ������ �����
//
void write_data(uint8_t);
uint8_t read_data(void);
void send_cmd(uint8_t);
void pulse_xtail(void);
//
void read_flash(void);
void write_flash(void);

void read_flash1(void);
void write_flash1(void);

#endif /* INC_PROGGER_H_ */
