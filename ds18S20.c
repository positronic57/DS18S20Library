/*
 * ds18s20.c
 *
 * Created: 13-Jul-15 23:49:17
 * Author: Goce Boshkovski
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License.
 */ 

#include <avr/io.h>
#include "ds18S20.h"
#include "CRCgen.h"
#include <util/delay.h>

/** @file ds18S20.c
 *  @brief Implements the functions defined in the header file.
 *
 *  @date 13-Jun-15
 *  @author Goce Boshkovski
 */

uint8_t OWReset(TSDS18S20 *pDS18S20)
{
	uint8_t result;

	*(pDS18S20->DS18S20_PORT-1) |= _BV(pDS18S20->DS18S20_PIN);
	_delay_us(480);
	
	*(pDS18S20->DS18S20_PORT-1) &= ~(_BV(pDS18S20->DS18S20_PIN));
	_delay_us(80);
	
	result = (*(pDS18S20->DS18S20_PORT-2)) & (_BV(pDS18S20->DS18S20_PIN));
	
	_delay_us(400);

	return result;
}

/* Send a bit over 1-Wire interface. */
void OWWriteBit(TSDS18S20 *pDS18S20, uint8_t bit)
{
	*(pDS18S20->DS18S20_PORT-1) |= _BV(pDS18S20->DS18S20_PIN);
	
	if (bit)
	{
		//Wite 1
		_delay_us(15);
		*(pDS18S20->DS18S20_PORT-1) &= ~(_BV(pDS18S20->DS18S20_PIN));
		_delay_us(45);
	}
	else
	{
		//Write 0
		_delay_us(60);
		*(pDS18S20->DS18S20_PORT-1) &= ~(_BV(pDS18S20->DS18S20_PIN));
	}

	return;
}

/* Read a bit from the 1-Wire interface. */
uint8_t OWReadBit(TSDS18S20 *pDS18S20)
{
	uint8_t result=0;

	*(pDS18S20->DS18S20_PORT-1) |= _BV(pDS18S20->DS18S20_PIN);
	_delay_us(4);
	
	*(pDS18S20->DS18S20_PORT-1) &= ~(_BV(pDS18S20->DS18S20_PIN));
	_delay_us(8);
	
	if ((*(pDS18S20->DS18S20_PORT-2) & (_BV(pDS18S20->DS18S20_PIN)))) 
		result=1;
	
	_delay_us(48);
	
	return result;
}

/* Send a byte over 1-Wire interface starting with most significant bit. */
void OWWriteByte(TSDS18S20 *pDS18S20, uint8_t value)
{
	uint8_t mask = 0x01;
	
	while (mask)
	{
		OWWriteBit(pDS18S20,value & mask);
		mask <<= 1;
	}
}

/* Read a byte form the 1-Wire interface. */
uint8_t OWReadByte(TSDS18S20 *pDS18S20)
{
	uint8_t data = 0x00;
	uint8_t mask = 0x01;
	
	while(mask)
	{
		if (OWReadBit(pDS18S20))
			data |= mask;
		mask <<= 1;
	}
	
	return data;
}

/* Init function for DS18S20. */
uint8_t DS18S20_Init(TSDS18S20 *pDS18S20,volatile uint8_t *DS18S20_PORT,uint8_t DS18S20_PIN)
{
	//Init ports/pins to which DS18S20 is attached.
	pDS18S20->DS18S20_PORT = DS18S20_PORT;
	pDS18S20->DS18S20_PIN = DS18S20_PIN;
	
	//Set DS18S20 PIN as input one and PORT bit to 0
	*(pDS18S20->DS18S20_PORT) &= ~(_BV(pDS18S20->DS18S20_PIN));
	*(pDS18S20->DS18S20_PORT-1) &= ~(_BV(pDS18S20->DS18S20_PIN));
	
	return OWReset(pDS18S20);
}

/* Reads DS18S20 64-bit ROM code without using the Search ROM procedure. */
uint8_t DS18S20_ReadROM(TSDS18S20 *pDS18S20)
{
	uint8_t i;
		
	OWWriteByte(pDS18S20,READ_ROM);

	for(i=0;i<8;i++)
		pDS18S20->serialNumber[i]=OWReadByte(pDS18S20);
	
	if (crc8(pDS18S20->serialNumber,7)==pDS18S20->serialNumber[7])
		return 1;
	else
		return 0;
}

/* This functions initiates a single temperature conversion. */
void DS18S20_MeasureTemperature(TSDS18S20 *pDS18S20)
{
	OWReset(pDS18S20);
	OWWriteByte(pDS18S20,SKIP_ROM);
	OWWriteByte(pDS18S20,CONVERT_T);
	
	//while(!OWReadBit(pDS18S20));
	_delay_ms(750);
	
	return;
}

/* Read the content of DS18S20 scratchpad and check the integrity with CRC. */
uint8_t DS18S20_ReadScratchPad(TSDS18S20 *pDS18S20)
{
	uint8_t i;
	
	OWReset(pDS18S20);
	OWWriteByte(pDS18S20,SKIP_ROM);	
	OWWriteByte(pDS18S20,READ_SCRATCHPAD);
	
	for(i=0;i<9;i++)
		pDS18S20->scratchpad[i]=OWReadByte(pDS18S20);
		
	if (crc8(pDS18S20->scratchpad,8)==pDS18S20->scratchpad[8])
		return 1;
	else
		return 0;
}

/* Returns the power supply type based on the respond from the sensor on Read Power Supply function command. */
uint8_t DS18S20_PowerSupplyType(TSDS18S20 *pDS18S20)
{
	OWReset(pDS18S20);
	OWWriteByte(pDS18S20,SKIP_ROM);
	OWWriteByte(pDS18S20,READ_POWER_SUPPLY);
	
	return OWReadBit(pDS18S20);	
}

/* Writes 2 bytes of data to the SA18S20 scratchpad (TH and TL registers).*/
void DS18S20_WriteScratchpad(TSDS18S20 *pDS18S20, uint8_t TH, uint8_t TL)
{
	OWReset(pDS18S20);
	OWWriteByte(pDS18S20,SKIP_ROM);
	OWWriteByte(pDS18S20,WRITE_SCRATCHPAD);
	
	OWWriteByte(pDS18S20,TH);
	OWWriteByte(pDS18S20,TL);

	return;
}

/* Copies the contents of the scratchpad TH and TL registers (bytes 2 and 3) to EEPROM. */
void DS18S20_CopyScratchpad(TSDS18S20 *pDS18S20)
{
	OWReset(pDS18S20);
	OWWriteByte(pDS18S20,SKIP_ROM);
	OWWriteByte(pDS18S20,COPY_SCRATCHPAD);
	
	while(!OWReadBit(pDS18S20));
		
	return;
}

/* Recalls the alarm trigger values (TH and TL) from EEPROM. */
void DS18S20_RECALL_E2(TSDS18S20 *pDS18S20)
{
	OWReset(pDS18S20);
	OWWriteByte(pDS18S20,SKIP_ROM);
	OWWriteByte(pDS18S20,RECALL_E2);
	
	while(!OWReadBit(pDS18S20));
	
	return;
}
