/*
 * main.c
 *
 * Copyright (c) 2015  Goce Boshkovski
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License.
 *
 * Example of using DS18S20 library on Atmega32 for temperature measurement. 
 * It demonstrates the usage of all functions present in the library.
 */



#include "OWcomm.h"
#include "ds18S20.h"
#include "UART.h"

int main(void)
{
	uint8_t code[8];	// buffer for the DS18S20 serial number (its address on 1-wire bus)
	char scratchpad[9];	// DS18S20 scratchpad
	uint8_t i;
	
	// AVR serail port init
	USART_init(103);
	
	// Pripremanje na ds18S20 za komunikacija
	if (OWReset())
	{
		USART_SendString("Error!!! Can not find DS18S20 device!\n");
		return -1;
	}
	else
		USART_SendString("Connected to DS1820 with serial number:");
	
	// Read DS18S20 address/serial number
	if (Send_READ_ROM_Command2DS18S20(code))
	{
		// Send the address via serial port
		for(i=1;i<7;i++)
		{
			USART_sendHex(code[i]);
			if (i==6) 
				USART_SendChar('\n');
			else
				USART_SendChar(':');
		}
	}
	else
		USART_SendString("CRC error!!!\n");
	
	// Initiate temperature conversion
	StartConvertTemperature();
	
	// Read DS18S20 scratchpad content to get the registries with temperature reading 
	OWReset();	// Reset DS18S20
	SendCommand2DS18S20(SKIP_ROM);	//  Send DS18S20 SKIP_ROM command
	if (ReadDS18S20ScratchPad(scratchpad))
	{
		// Send the value of the temperature registers over serial port
		USART_SendString("Current Temperature is:");
		ConvertTemperature2String(scratchpad[1],scratchpad[0]);
	}
	else
		USART_SendString("CRC error!!!\n");
	
	while(1);
		
	return 0;

}
