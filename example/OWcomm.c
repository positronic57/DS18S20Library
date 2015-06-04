/*
*
* AVR-libc implementaion of 1-Wire interface.
* 
* Copyright (c) 2013 Goce Boshkovski
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License.
*
*/

#include "OWcomm.h"
#include <avr/interrupt.h>

uint8_t OWReset()
{
	uint8_t result;
	
	DS18S20_PORT &= ~(_BV(DS18S20_DQ));
	DS18S20_DDR |= _BV(DS18S20_DQ);
	delay_us(480);
	
	DS18S20_DDR &= ~(_BV(DS18S20_DQ));
	delay_us(70);
	
	result = DS18S20_PIN & (_BV(DS18S20_DQ)); 
	
	delay_us(412);
	
	return result;
}
	
/* Send a bit over 1-Wire interface. */	
void OWWriteBit(uint8_t bit)
{
	uint8_t sreg;
	
	sreg=SREG;	
	cli();	
	
	DS18S20_PORT &= ~(_BV(DS18S20_DQ));
	
	if (bit)
	{
		//Wite 1
		DS18S20_DDR |= _BV(DS18S20_DQ);
		delay_us(6);	
		DS18S20_DDR &= ~(_BV(DS18S20_DQ));
		delay_us(62);
	}
	else
	{
		//Write 0
		DS18S20_DDR |= _BV(DS18S20_DQ);
		delay_us(70);
		DDRD &= 0xDF;
	}
	
	sei();	
	SREG=sreg;	
}
		
/* Read a bit from the 1-Wire interface. */
uint8_t OWReadBit()
{
	uint8_t result=0;
	uint8_t sreg;
	
	sreg=SREG; 
	cli(); 
			

	DS18S20_PORT &= ~(_BV(DS18S20_DQ));
	DS18S20_DDR |= _BV(DS18S20_DQ);
	delay_us(6);
	
	DS18S20_DDR &= ~(_BV(DS18S20_DQ));
	delay_us(9);
	
	if (DS18S20_PIN & (_BV(DS18S20_DQ))) result=1;
	delay_us(60);
	
	sei();
	SREG=sreg;	
	
	return result;
}

/* Send a byte over 1-Wire interface starting with most significant bit. */
void OWWriteByte(uint8_t data)
{
	uint8_t mask = 0x01;
	
	while (mask)
	{
		OWWriteBit(data & mask);
		mask <<= 1;
	}
}

/* Read a byte form the 1-Wire interface. */
uint8_t OWReadByte(void)
{
	uint8_t data = 0x00;
	uint8_t mask = 0x01;
	
	while(mask)
	{
		if (OWReadBit())
			data |= mask;
		mask <<= 1;
	}
			
	return data;
}


