/*
 * DS18B20example3.c
 *
 * Created: 15-Aug-15 22:39:32
 * Author: Goce Boshkovski
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License.
 *
 * The program demonstrates the usage of DS18S20 library with Atmega32
 * for temperature measurement. The temperature reading from the sensor is
 * displayed on the connected HD44780 LCD module. 
 * It uses HD44780library (https://github.com/positronic57/HD44780Library) 
 * for driving the LCD module.
 *
 * The CPU freq is 4MHz and must be defined via F_CPU symbol during the compiling time using 
 * the -DF_CPU arrgument.
 * 
 * DS18B20 and Atmega32 connection: 
 * - DQ DS18S20 connected on PIN4 of PORTC.
 * 
 * DS18B20 is powered by an external supply.
 * There is a pull-up resistor on the 1-Wire bus.
 *
 * Connection between MCU and JHD 162A 16x2 LCD module:
 * - PORTB0 -> LCD D4;
 * - PORTB1 -> LCD D5;
 * - PORTB2 -> LCD D6;
 * - PORTB3 -> LCD D7;
 * - PORTD4 -> LCD RS;
 * - PORTD6 -> LCD R/W;
 * - PORTD7 -> LCD E.
*/ 

#include <avr/io.h>
#include <stdlib.h>
#include "HD44780.h"
#include "ds18S20.h"

int main(void)
{
	TSHD44780 HD44780Display;
	TSHD44780 *pHD44780Display = &HD44780Display;
	
	TSDS18x20 DS18x20;
	TSDS18x20 *pDS18x20 = &DS18x20;
	char buffer[10];

	// LCD Init.
	LCDInit4(pHD44780Display,&PORTD,&PORTB,PD4,PD7,PD6,TWO_LINES_DISPLAY);
	LCDSendCommand4(pHD44780Display,DISPLAY_ON);
	
	if (DS18x20_Init(pDS18x20,&PORTC,PC4))
	{
		LCDShowString4(pHD44780Display,"Error!!!");
		return -1;
	}
	else
		LCDShowString4(pHD44780Display,"T=");
		
	// Set DS18B20 resolution to 12 bits.
	DS18x20_SetResolution(pDS18x20,CONF_RES_12b);
	DS18x20_WriteScratchpad(pDS18x20);

	// Initiate a temperature conversion and get the temperature reading
	if (DS18x20_MeasureTemperature(pDS18x20))
	{
		// Show the temperature on the LCD module.
		dtostrf(DS18x20_TemperatureValue(pDS18x20),7,4,buffer);
		LCDShowString4(pHD44780Display,buffer);
		LCDShowCharacter4(pHD44780Display,'C');
	}
	else
		LCDShowString4(pHD44780Display,"CRC error!!!");
	
    while(1)
    {

    }
}
