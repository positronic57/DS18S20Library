/*
 * DS18B20example2.c
 *
 * Created: 11-Aug-15 23:36:05
 * Author: Goce Boshkovski
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License.
 *
 * This program demonstrates how DS18S20 library can be use for reading
 * DS18B20 ROM code and check power supply type of the sensor.
 * The sensor is connected to Atmega32 microcontroller.
 * 
 * The CPU freq is 16MHz and must be defined via F_CPU symbol during the compiling time using 
 * the -DF_CPU arrgument.
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

int main(void)
{
	TSDS18x20 DS18x20;
	TSDS18x20 *pDS18x20 = &DS18x20;
	uint8_t i;
	
	// AVR serial port init
	USART_init(103);
	
	// Init DS18B20 sensor
	if (DS18x20_Init(pDS18x20,&PORTD,PD5))
	{
		USART_SendString("Error!!! Can not find 1-Wire device attached on the bus!");
		return -1;
	}
	else
		USART_SendString("1-Wire device detected on the bus.");
	USART_SendChar(0x0D);

	// Read the ROM code of the sensor and send it via serial port 
	USART_SendString("The ROM address of the device is: ");
	if (DS18x20_ReadROM(pDS18x20))
	{
		// Send the sensor address via serial port
		for(i=12;i>1;i--)
		{
			if (i%2)
			USART_SendChar(':');
			else
			USART_sendHex(pDS18x20->serialNumber[i/2]);
		}
		USART_SendChar(0x0D);
	}
	else
		USART_SendString("CRC error!!!");
	
	// Check sensors power supply type
	if (DS18x20_PowerSupplyType(pDS18x20))
		USART_SendString("The sensor is externally powered.");
	else
		USART_SendString("The sensor is parasite powered.");		
	USART_SendChar(0x0D);
	
	
	while(1);
	
	return 0;

}
