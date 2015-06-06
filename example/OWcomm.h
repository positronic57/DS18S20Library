/*
* 
* AVR-libc implementaion of 1-Wire interface.
* 
* Copyright (c) 2013 Goce Boshkovski
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License.
*
*/
#ifndef OWCOMM_H_
#define OWCOMM_H_

#include <avr/io.h>
#include "config.h"

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#ifndef DS18S20_PORT_DEF
#define DS18S20_PORT_DEF D
#endif

#ifndef DS18S20_DQ
#define DS18S20_DQ PORT0
#endif

#define GLUE(a,b) a##b
#define PORT(x) GLUE(PORT,x)
#define DDR(x) GLUE(DDR,x)
#define PIN(x) GLUE(PIN,x)

#define DS18S20_PORT PORT(DS18S20_PORT_DEF)
#define DS18S20_DDR DDR(DS18S20_PORT_DEF)
#define DS18S20_PIN PIN(DS18S20_PORT_DEF)

#include "delay.h"

// Write bit/byte on 1-Wire interface
extern void OWWriteBit(uint8_t);
extern void OWWriteByte(uint8_t);

// Read bit/byte from 1-Wire interface
extern uint8_t OWReadBit();
extern uint8_t OWReadByte();

extern uint8_t OWReset();

#endif /* OWCOMM_H_ */
