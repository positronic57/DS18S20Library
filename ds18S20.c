/*
* 
* DS18S20 temprature sensor AVR-libc library for AVR 8-bit 
* microcontrollers.
* 
* Copyright (c) 2013 Goce Boshkovski
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License.
*
*/

#include "ds18S20.h"
#include "CRCgen.h"

// Send command to DS18x20 over 1-Wire interface.
void SendCommand2DS18S20(uint8_t command)
{
	OWWriteByte(command);
}

/* Read DS18x20 ROM memory. */
uint8_t Send_READ_ROM_Command2DS18S20(uint8_t *code)
{
	uint8_t i;
	uint8_t *temp;
	
	temp=code;
	OWWriteByte(READ_ROM);
	
	for(i=0;i<8;i++)
		*code++=OWReadByte();
	if (crc8(temp,7)==*(temp+7))
		return 1;
	else
		return 0;
	
}

// Start temperature conversion.
void StartConvertTemperature()
{
	uint8_t sreg;
		
	// Sochuvaj ja sostojbata na SREG i onevozmozi interapt
	sreg=SREG;
	cli();
	
	// Isprati naredba za konverzija
	OWWriteByte(CONVERT_T);
	
	// Konfiguriranje na portot PD5 kako vlezen portot
	DS18S20_PORT &= ~(_BV(DS18S20_DQ));
	DS18S20_DDR &= ~(_BV(DS18S20_DQ));
	
	// Chekaj dodeka ne zavrshi konverzijata.
	while(!(DS18S20_PIN & _BV(DS18S20_DQ)));
	
	sei();
	SREG=sreg;
}

/* Read DS18x20 scratch pad and check the integrity with CRC. */
uint8_t ReadDS18S20ScratchPad(uint8_t *scratchpad)
{
	uint8_t i;
	uint8_t *temp;
	
	temp=scratchpad;
	OWWriteByte(READ_SCRATCHPAD);
	
	for(i=0;i<9;i++)
		*scratchpad++=OWReadByte();
	if (crc8(temp,8)==*(temp+8))
		return 1;
	else
		return 0;

}
