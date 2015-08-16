/*
 * DS18B20example1.c
 *
 * Created: 11-Aug-15 23:21:33
 * Author: Goce Boshkovski
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License.
 *
 * This program demonstrates the usage of DS18S20 library with Atmega32
 * for temperature measurement. The temperature reading from the sensor is
 * first converted to a decimal value using DS18x20_TemperatureValue() function
 * from the library and then to a string by dtostrf().This function is located 
 * in the mathematical library, libm.a.
 * Link this example with the math library with the -lm option while compiling.
 * 
 * The CPU freq is 16MHz defined with F_CPU.
 * 
 * DS18S20 and Atmega32 connection: 
 * - DQ DS18S20 connected to PIN5 of PORTD
 * 
 * DS18S20 is powered by an external supply.
 * There is a pull-up resistor on the 1-Wire bus.
 */ 


#include <avr/io.h>
#include <stdlib.h>

#define F_CPU 16000000UL

#include <stdlib.h>
#include "ds18s20.h"
#include "UART.h"

int main(void)
{
	TSDS18x20 DS18x20;
	TSDS18x20 *pDS18x20 = &DS18x20;
	char buffer[10];
	uint8_t i;
	
	// AVR serial port init
	USART_init(103);
	
	// Init DS18B20 sensor
	if (DS18x20_Init(pDS18x20,DS18B20Sensor,&PORTD,PD5))
	{
		USART_SendString("Error!!! Can not find 1-Wire device attached on the bus!");
		return -1;
	}
	else
		USART_SendString("1-Wire device detected on the bus.");
	USART_SendChar(0x0D);

	// Set DS18B20 resolution to 9 bits.
	DS18x20_SetResolution(pDS18x20,CONF_RES_9b);
	DS18x20_WriteScratchpad(pDS18x20);
	
	// Initiate a temperature conversion
	DS18x20_MeasureTemperature(pDS18x20);
	
	// Read sensor memory to fetch temperature reading	
	if (DS18x20_ReadScratchPad(pDS18x20))
	{
		// Send the temperature over serial port
		USART_SendString("Current Temperature is: ");
		dtostrf(DS18x20_TemperatureValue(pDS18x20),9,4,buffer);
		USART_SendString(buffer);
		USART_SendChar('C');
		USART_SendChar(0x0D);
	}
	else
		USART_SendString("CRC error!!!");
	
	while(1);
	
	return 0;

}
