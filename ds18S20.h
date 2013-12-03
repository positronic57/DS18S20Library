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

#include "OWcomm.h"
#include <avr/io.h>
#include <avr/interrupt.h>

// DS18S20 ROM Commands
#define SEARCH_ROM 0xF0
#define READ_ROM 0x33
#define MATCH_ROM 0x55
#define SKIP_ROM 0xCC
#define ALARM_SEARCH 0xEC

// DS18S20 Function Commands
#define CONVERT_T 0x44
#define WRITE_SCRATCHPAD 0x4E
#define READ_SCRATCHPAD 0xBE
#define COPY_SCRATCHPAD 0x48
#define RECALL_E2 0xB8
#define READ_POWER_SUPPLAY 0xB4
 
extern void SendCommand2DS18S20(uint8_t);
extern uint8_t Send_READ_ROM_Command2DS18S20(uint8_t *);
extern void StartConvertTemperature();
extern uint8_t ReadDS18S20ScratchPad(uint8_t *);






