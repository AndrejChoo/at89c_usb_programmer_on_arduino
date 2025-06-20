/*
 * progger.c
 *
 *  Created on: 10 июн. 2025 г.
 *      Author: andre
 */

#include "progger.h"


extern uint8_t exchange[];
extern uint8_t main_buff[];
extern volatile uint8_t nxt;
uint16_t fsize = 0, offset = 0;
uint8_t dev_type;

static void hc595_write(uint8_t);
static void send_address(uint16_t);


static void hc595_write(uint8_t dat)
{
	SCK_LOW
	for(int i=0; i<8; i++)
	{
		if((dat>>(7-i))&0x01) MOSI_HIGH
		else MOSI_LOW
		SCK_HIGH
		SCK_LOW
	}
}

static void send_address(uint16_t add)
{
	hc595_write(add >> 8);
	hc595_write(add & 0xFF);
	LATCH_HIGH
	LATCH_LOW
}

void send_successConn(void)
{
	exchange[0] = 0xAC;
	exchange[1] = 0x00;
	exchange[2] = 0x01;
	exchange[3] = 0xDA;
	exchange[4] = 0x00;
	send_frame(exchange, 5);
}

void send_frame(uint8_t *buff, uint16_t count)
{
	for(uint16_t n = 0; n < count; n++) usart_sendChar(buff[n]);
}

//R/W Data
void write_data(uint8_t dat)
{
	PORTB &= 0b11000000;
	PORTB |= (dat & 0b00111111);

	PORTD &= 0b00111111;
	PORTD |= (dat & 0b11000000);
}

uint8_t read_data(void)
{
	return ((PIND & 0b11000000)|(PINB & 0b00111111));
}

void send_cmd(uint8_t cmd)
{
	PORTC &= 0b11110000;
	PORTC |= (cmd & 0x0F);
}

void pulse_xtail(void)
{
	_delay_us(1);
	XT1_HIGH
	_delay_us(1);
	XT1_LOW
	_delay_us(1);
}

//
void deassemble_frame(void)
{
	switch(exchange[3])
	{
		//Check programmer
		case 0xDA:
		{
			send_successConn();
			break;
		}
		//Read ID
		case 0xA0:
		{
			dev_type = exchange[4];
			data_in();
			VPP_LOW
			_delay_ms(10);
			VPP_HIGH

			if(dev_type == 0) //at89cx051
			{
				PROG_HIGH
				send_cmd(RDSIGN);
				_delay_ms(10);
				main_buff[6] = read_data();
				pulse_xtail();
				_delay_us(10);
				main_buff[5] = read_data();
				main_buff[4] = 0xFF;
			}
			else
			{
				PROG_HIGH
				send_address(0x30);
				send_cmd(RDSIGN);
				_delay_us(10);
				main_buff[6] = read_data();

				send_address(0x31);
				send_cmd(RDSIGN);
				_delay_us(10);
				main_buff[5] = read_data();

				send_address(0x32);
				send_cmd(RDSIGN);
				_delay_us(10);
				main_buff[4] = read_data();
			}

			main_buff[0] = 0xAC;
			main_buff[1] = 0x00;
			main_buff[2] = 0x03;
			main_buff[3] = 0xA0;
			send_frame(main_buff, 7);

			break;
		}
		//Write Lock
		case 0xA2:
		{
			dev_type = exchange[8];
			data_out();
			write_data(0xFF);
			VPP_LOW
			_delay_ms(10);
			VPP_HIGH

			if(dev_type == 0)
			{
				VPP_PROG
				_delay_ms(10);
				//Bit0
				if(!(exchange[7] & 0x01))
				{
					send_cmd(WRLOCK1);
					_delay_us(10);
					PROG_LOW
					_delay_us(120);
					PROG_HIGH
					_delay_ms(2);
				}
				//Bit1
				if(!(exchange[7] & 0x02))
				{
					send_cmd(WRLOCK2);
					_delay_us(10);
					PROG_LOW
					_delay_us(120);
					PROG_HIGH
					_delay_ms(2);
				}
				VPP_HIGH
			}
			else
			{
				PROG_HIGH
				VPP_PROG

				//Bit0
				if(!(exchange[7] & 0x01))
				{
					send_address(0);
					send_cmd(WRLOCK1);
					_delay_us(10);
					PROG_LOW
					_delay_us(120);
					PROG_HIGH
					_delay_ms(2);
				}
				//Bit1
				if(!(exchange[7] & 0x02))
				{
					send_address(0);
					send_cmd(WRLOCK2);
					_delay_us(10);
					PROG_LOW
					_delay_us(120);
					PROG_HIGH
					_delay_ms(2);
				}
				//Bit2
				if(!(exchange[7] & 0x04))
				{
					send_address(0);
					send_cmd(WRLOCK3);
					_delay_us(10);
					PROG_LOW
					_delay_us(120);
					PROG_HIGH
					_delay_ms(2);
				}
				VPP_HIGH
			}

        	main_buff[0] = 0xAC;
        	main_buff[1] = 0x00;
        	main_buff[2] = 0x01;
        	main_buff[3] = 0xA2;
        	main_buff[4] = 0x00;
        	send_frame(main_buff, 5);
			break;
		}
		//Erase
		case 0xA3:
		{
			dev_type = exchange[4];
			data_out();
			write_data(0xFF);
			VPP_LOW
			_delay_ms(10);
			VPP_PROG
			_delay_ms(10);

			if(dev_type == 0)
			{
				send_cmd(ERASE);
				_delay_ms(10);
				PROG_LOW
				_delay_ms(15);
				PROG_HIGH
				_delay_ms(20);
				VPP_HIGH
			}
			else
			{
				send_address(0);
				send_cmd(ERASE);
				_delay_ms(10);
				PROG_LOW
				_delay_ms(15);
				PROG_HIGH
				_delay_ms(100);
				VPP_HIGH
			}

        	main_buff[0] = 0xAC;
        	main_buff[1] = 0x00;
        	main_buff[2] = 0x01;
        	main_buff[3] = 0xA3;
        	main_buff[4] = 0xA1;
        	send_frame(main_buff, 5);
			break;
		}
		//IDDLE
		case 0xA7:
		{
			VPP_HIGH
			data_in();
			break;
		}
		//Read
		case 0xA8:
		{
			fsize = exchange[4] | (exchange[5] << 8);
			dev_type = exchange[7];

			if(dev_type == 0) read_flash();
			else  read_flash1();

			break;
		}
		//Prepare to write
		case 0xA9:
		{
			data_out();
			dev_type = exchange[6];
			offset = 0;

			if(dev_type == 0)
			{
				write_data(0xFF);
				PROG_HIGH
				VPP_LOW
				_delay_ms(10);
				VPP_PROG
				_delay_ms(10);
				send_cmd(WRCODE);
			}
			else
			{
				PROG_HIGH
				VPP_LOW
				_delay_ms(10);
				VPP_PROG
				_delay_ms(10);
			}

        	main_buff[0] = 0xAC;
        	main_buff[1] = 0x00;
        	main_buff[2] = 0x01;
        	main_buff[3] = 0xAA;
        	main_buff[4] = 0x00;
        	send_frame(main_buff, 5);
			break;
		}
		//Write
		case 0xAA:
		{
			if(dev_type == 0) write_flash();
			else write_flash1();

			break;
		}
		default:
		{
			main_buff[0] = 0xAC;
			main_buff[1] = 0x00;
			main_buff[2] = 0x01;
			main_buff[3] = 0xEE;
			main_buff[4] = 0x00;
			send_frame(main_buff, 5);
			break;
		}
	}
}


void read_flash(void)
{
	data_in();
	send_cmd(RDCODE);
	PROG_HIGH
	VPP_LOW
	_delay_ms(20);
	VPP_HIGH

	for(int i = 0; i < fsize; i+= 512)
	{
        //Читаем 512 байт
		for(int k = 0; k < 512; k++)
		{
			_delay_us(10);
			main_buff[k + 4] = read_data();
			pulse_xtail();
		}

		main_buff[0] = 0xAC;
		main_buff[1] = 0x02;
		main_buff[2] = 0x00;
		main_buff[3] = 0xA8;
		send_frame(main_buff, 516);

		while(!nxt);
		nxt = 0;
	}
}

void write_flash(void)
{
	for(uint16_t i = 0; i < 512; i++)
	{

		if(exchange[i + 7] == 0xFF)
			{
				pulse_xtail();
				continue;
			}

		write_data(exchange[i + 7]);
		_delay_us(10);
		PROG_LOW
		_delay_us(120);
		PROG_HIGH
		_delay_us(10);
		while(!READ_BSY);
		pulse_xtail();
	}

	main_buff[0] = 0xAC;
	main_buff[1] = 0x00;
	main_buff[2] = 0x01;
	main_buff[3] = 0xAA;
	main_buff[4] = 0x00;
	send_frame(main_buff, 5);
}

void read_flash1(void)
{
	data_in();
	PROG_HIGH
	VPP_LOW
	_delay_ms(20);
	VPP_HIGH

	for(int i = 0; i < fsize; i+= 512)
	{
        //Читаем 512 байт
		for(int k = 0; k < 512; k++)
		{
			send_address(i + k);
			send_cmd(RDCODE);
			_delay_us(10);
			main_buff[k + 4] = read_data();
		}

		main_buff[0] = 0xAC;
		main_buff[1] = 0x02;
		main_buff[2] = 0x00;
		main_buff[3] = 0xA8;
		send_frame(main_buff, 516);

		while(!nxt);
		nxt = 0;
	}
}

void write_flash1(void)
{
	for(uint16_t i = 0; i < 512; i++)
	{

		if(exchange[i + 7] == 0xFF)
		{
			continue;
		}
		send_address(i + offset);
		send_cmd(WRCODE);
		write_data(exchange[i + 7]);
		_delay_us(10);
		PROG_LOW
		_delay_us(120);
		PROG_HIGH
		_delay_us(10);
		while(!READ_BSY);
	}
	offset += 512;

	main_buff[0] = 0xAC;
	main_buff[1] = 0x00;
	main_buff[2] = 0x01;
	main_buff[3] = 0xAA;
	main_buff[4] = 0x00;
	send_frame(main_buff, 5);
}







