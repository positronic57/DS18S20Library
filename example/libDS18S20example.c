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
 * This program demonstrates the usage of DS18S20 library with Atmega32
 * for temperature measurement. The temperature reading from the sensor is
 * converted to a string with additional function that is not part of the
 * library and send via serial port.
 * 
 * DS18S20 and Atmega32 connection: 
 * - DQ DS18S20 connected to PIN5 of PORTD
 * 
 * DS18S20 is powered by an external supply.
 * There is a pull-up resistor on the 1-Wire bus (DQ Pin).
 */ 


#include <avr/io.h>
#include <string.h>

#define F_CPU 16000000UL

#include "ds18s20.h"
#include "UART.h"

//Calculates the temperature and sends it via serial port
void ConvertTemperature2String(uint8_t LSB, uint8_t MSB, char *buffer);

int main(void)
{
	TSDS18S20 DS18S20;
	TSDS18S20 *pDS18S20 = &DS18S20;
	char buffer[9];
		
	uint8_t i;
	
	//AVR serial port init
	USART_init(103);
	
	// Sensor init
	if (DS18S20_Init(pDS18S20,&PORTD,PD5))
	{
		USART_SendString("Error!!! Can not find 1-Wire device attached on the bus!");
		return -1;
	}
	else
		USART_SendString("1-Wire device detected on the bus.");
	USART_SendChar(0x0D);

	USART_SendString("The ROM code of the device is: ");
	// Read the ROM code of the sensor and send it via serial port
	if (DS18S20_ReadROM(pDS18S20))
	{
		for(i=12;i>1;i--)
		{
			if (i%2)
				USART_SendChar(':');
			else
				USART_sendHex(pDS18S20->serialNumber[i/2]);
		}
		USART_SendChar(0x0D);
	}
	else
		USART_SendString("CRC error!!!");
	
	// Check sensors power supply type
	if (DS18S20_PowerSupplyType(pDS18S20))
		USART_SendString("The sensor is externally powered.");
	else
		USART_SendString("The sensor is parasite powered.");		
	USART_SendChar(0x0D);
	
	// Initiate a temperature conversion
	DS18S20_MeasureTemperature(pDS18S20);
		
	// Read sensor memory to fetch temperature reading
	if (DS18S20_ReadScratchPad(pDS18S20))
	{
		// Send the value of the temperature registers over serial port
		USART_SendString("Current Temperature is: ");
		ConvertTemperature2String(pDS18S20->scratchpad[0],pDS18S20->scratchpad[1],buffer);
		USART_SendString(buffer);
		USART_SendChar('C');
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
		strcat(buffer,".5");
	else
		strcat(buffer,".0");
	
	return;
}
