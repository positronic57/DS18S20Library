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
void ConvertTemperature2String(uint8_t LSB, uint8_t MSB);

int main(void)
{
	TSDS18x20 DS18S20;
	TSDS18x20 *pDS18S20 = &DS18S20;
	
	uint8_t i;
	
	//AVR serial port init
	USART_init(103);
	
	if (DS18x20_Init(pDS18S20,DS18S20Sensor,&PORTD,PD5))
	{
		USART_SendString("Error!!! Can not find DS18S20 device!");
		return -1;
	}
	else
		USART_SendString("Connected to DS1820 with serial number:");
	USART_SendChar(0x0D);

	if (DS18x20_ReadROM(pDS18S20))
	{
		// Send the sensor address via serial port
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
	
	if (DS18x20_PowerSupplyType(pDS18S20))
		USART_SendString("The sensor is externally powered.");
	else
		USART_SendString("The sensor is parasite powered.");		
	USART_SendChar(0x0D);
	
	DS18x20_CopyScratchpad(pDS18S20);
	
	DS18x20_MeasureTemperature(pDS18S20);
		
	if (DS18x20_ReadScratchPad(pDS18S20))
	{
		// Send the value of the temperature registers over serial port
		USART_SendString("Current Temperature is:");
		ConvertTemperature2String(pDS18S20->scratchpad[0],pDS18S20->scratchpad[1]);
		USART_SendChar(0x0D);
	}
	else
		USART_SendString("CRC error!!!");
	
	while(1);
	
	return 0;

}

void ConvertTemperature2String(uint8_t LSB, uint8_t MSB)
{
	uint8_t	temp = LSB;
	uint8_t delitel = 100;
	
	if (MSB)
	{
		/* LSB represents number of 0.5C in actual temperature value. 
		 * LSB uses two complement format for representing signed integer values.
		 */
		temp = ~temp + 1;
		USART_SendChar('-');
	}
	else
		USART_SendChar('+');

	temp >>= 1;
	//Eliminate leading zeros
	while(!(temp/delitel))
		delitel/=10;
	do
	{
		USART_SendChar('0'+temp/delitel);
		temp %= delitel;
		delitel /= 10;
	}while(temp);
	
	USART_SendChar('.');
	if (LSB & 0x01)
		USART_SendChar('5');
	else
		USART_SendChar('0');
	
	USART_SendChar('C');
	USART_SendChar(0x0D);
	
}
