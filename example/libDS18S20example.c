/*
 * libDS18S20example.c
 *
 * Created: 13-Jul-15 23:39:35
 * Author: Goce Boshkovski
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License.
 *
 * This program demonstrates the usage of DS18S20 library with Atmega32.
 * The CPU freq is 16MHz defined with F_CPU.
 * 
 * DS18S20 and Atmega32 connection: 
 * - DQ DS18S20 connected to PIN5 of PORTD
 * 
 * DS18S20 is powered by an external supply.
 * There is a pull-up resistor on the 1-Wire bus.
 */ 


#include <avr/io.h>

#define F_CPU 16000000UL

#include "ds18s20.h"
#include "UART.h"

//Calculates the temperature and sends it via serial port
void ConvertTemperature2String(char,char);

int main(void)
{
	TSDS18S20 DS18S20;
	TSDS18S20 *pDS18S20 = &DS18S20;
	
	uint8_t i;
	
	// AVR serial port init.
	USART_init(103);
	
	if (DS18S20_Init(pDS18S20,&PORTD,PD5))
	{
		USART_SendString("Error!!! Can not find DS18S20 device!");
		return -1;
	}
	else
		USART_SendString("Connected to DS1820 with serial number:");
	USART_SendChar(0x0D);

	//Read 64-bit ROM code without using the Search ROM procedure. 
	if (DS18S20_ReadROM(pDS18S20))
	{
		// Send the sensor address via serial port.
		for(i=1;i<7;i++)
		{
			USART_sendHex(pDS18S20->serialNumber[i]);
			if (i==6)
				USART_SendChar(0x0D);
			else
				USART_SendChar(':');
		}
	}
	else
		USART_SendString("CRC error!!!");
	
	// Initiate temperature conversion.
	DS18S20_MeasureTemperature(pDS18S20);
		
	// Read DS18S20 scratchpad content to get the registries with temperature reading.
	if (DS18S20_ReadScratchPad(pDS18S20))
	{
		// Send the value of the temperature registers over serial port.
		USART_SendString("Current Temperature is:");
		ConvertTemperature2String(pDS18S20->scratchpad[1],pDS18S20->scratchpad[0]);
		USART_SendChar(0x0D);
	}
	else
		USART_SendString("CRC error!!!");
	
	while(1);
	
	return 0;

}

void ConvertTemperature2String(char TH,char TL)
{
	char delitel=10;
	char tmp;

	tmp=TL;
	TL >>=1;
	TL &=0x7F;
	if (TH)
	{
		TL=127-TL;
		USART_SendChar('-');
	}
	else
	USART_SendChar('+');
	if (TL==-1)
	USART_SendChar('0');
	else
	{
		do
		{
			USART_SendChar('0'+TL/delitel);
			TL%=delitel;
			delitel/=10;
		}while(TL);
	}
	USART_SendChar('.');
	if (tmp&1)
		USART_SendChar('5');
	else
		USART_SendChar('0');
	USART_SendChar('C');
	USART_SendChar(0x0D);
	
}
