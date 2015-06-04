/*
 * UART.c
 *
 * Copyright (c) 2012  Goce Boshkovski
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License.
 *
 */



#include "UART.h"


void USART_init(unsigned int baud)
{
	UBRRH = (unsigned char)(baud>>8);
	UBRRL = (unsigned char)baud;

	// Enable Rx and Tx and RX interrupt
	UCSRB = (1 << RXEN) | (1 << TXEN);
	
	// Set frame format: 8data, 1stop bit 
	UCSRC = (1<<URSEL)|(3<<UCSZ0);
}

void USART_SendChar(unsigned char c)
{
	while(!(UCSRA & (1 << UDRE)));
	UDR = c;
}

void USART_SendString(char * s)
{
	while(*s)
	{
		USART_SendChar(*s);
		s++;
	}
}

void USART_sendHex(uint8_t znak)
{
	
	unsigned char mn = 0xF0;	
	unsigned char ln = 0x0F;	

	mn &= znak;
	mn >>= 4;
	
	ln &= znak;
	
	if (mn > 9)
		mn = 'A'+mn-10;
	else
		mn += '0';
	if (ln > 9)
		ln ='A'+ln-10;
	else
		ln += '0';
	
	USART_SendChar(mn);
	USART_SendChar(ln);
	
	return;
}
