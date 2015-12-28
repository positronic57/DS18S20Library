/*
 * DS18S20example1.c
 *
 * Created: 11-Aug-15 23:36:35
 * Author: Goce Boshkovski
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License.
 *
 * This program demonstrates the usage of DS18S20 library with Atmega32
 * for temperature measurement. The temperature reading from the sensor is
 * converted to a string with additional function that is not part of the
 * library and send via serial port.
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
#include <string.h>
#include <stdlib.h>

#include "ds18S20.h"
#include "UART.h"

// Convert the temperature reading to a string
void ConvertTemperature2String(uint8_t LSB, uint8_t MSB, char *buffer);

int main(void)
{
	TSDS18x20 DS18x20;
	TSDS18x20 *pDS18x20 = &DS18x20;
	char buffer[10];
	
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

	// Initiate a temperature conversion and get the temperature reading
	if (DS18x20_MeasureTemperature(pDS18x20))
	{
		// Send the temperature over serial port
		USART_SendString("Current Temperature is: ");
		ConvertTemperature2String(pDS18x20->scratchpad[0],pDS18x20->scratchpad[1],buffer);
		USART_SendString(buffer);
		USART_SendChar(0x0D);
	}
	else
		USART_SendString("CRC error!!!");
	
	while(1);
	
	return 0;

}

// Convert sensor reading to a string
void ConvertTemperature2String(uint8_t LSB, uint8_t MSB, char *buffer)
{
	int16_t Temperature=0;
	char hp[3];
	
	Temperature = MSB;
	Temperature <<= 8;
	Temperature |= LSB;
	
	if (Temperature<0)
		*buffer='-';
		
	itoa(abs(Temperature)/2,hp,10);
	strcat(buffer,hp);
	
	if (LSB & 0x01)
		strcat(buffer,".5C");
	else
		strcat(buffer,".0C");
	
	return;
}
