/*
 * ds18s20.h
 *
 * Created: 13-Jul-15 23:48:08
 * Author: Goce Boshkovski
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License.
 */ 


#ifndef DS18S20_H_
#define DS18S20_H_

#include <stdint.h>

/**
 * @file ds18S20.h
 * @brief 
 * Header file of DS18S20 library. It contains the prototypes of all
 * functions available in the library, definitions of all macros
 * and constans.
 * 
 * @author Goce Boshkovski
 * @date 13-Jun-15
 * @copyright GNU General Public License v2.
 * 
 */

//Define MCU CPU Freq for the time delay functions
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

/** \defgroup time Time delay functions and macros (Precise Delay Functions V 0.5, Martin Thomas, 9/2004)*/
/* @{ */
/**
 * @author Martin Thomas
 * @brief Precise Delay Functions
 * V 0.5, Martin Thomas, 9/2004
 */
static inline void delayloop16(uint16_t count)
{
	asm volatile (  "cp  %A0,__zero_reg__ \n\t"  \
	"cpc %B0,__zero_reg__ \n\t"  \
	"breq L_Exit_%=       \n\t"  \
	"L_LOOP_%=:           \n\t"  \
	"sbiw %0,1            \n\t"  \
	"brne L_LOOP_%=       \n\t"  \
	"L_Exit_%=:           \n\t"  \
	: "=w" (count)
	: "0"  (count)
	);
}
/**
 * @author Martin Thomas
 * @brief Precise Delay Functions
 * V 0.5, Martin Thomas, 9/2004
 */
#define DELAY_US_CONV(us) ((uint16_t)(((((us)*1000L)/(1000000000/F_CPU))-1)/4))
/**
 * @author Martin Thomas
 * @brief Precise Delay Functions
 * V 0.5, Martin Thomas, 9/2004
 */
#define delay_us(us)	  delayloop16(DELAY_US_CONV(us))
/**
 * @author Martin Thomas
 * @brief Precise Delay Functions
 * V 0.5, Martin Thomas, 9/2004
 */
void delayloop32( uint32_t l);
/**
 * @author Martin Thomas
 * @brief Precise Delay Functions
 * V 0.5, Martin Thomas, 9/2004
 */
#define DELAY_MS_CONV(ms) ( (uint32_t) (ms*(F_CPU/6000L)) ) 
/**
 * @author Martin Thomas
 * @brief Precise Delay Functions
 * V 0.5, Martin Thomas, 9/2004
 */
#define delay_ms(ms)  delayloop32(DELAY_MS_CONV(ms))

void delayloop32(uint32_t loops);
/* @} */


/** \defgroup DS18S20ROMCmd DS18S20 ROM Commands */
/* @{ */
#define SEARCH_ROM 0xF0
#define READ_ROM 0x33
#define MATCH_ROM 0x55
#define SKIP_ROM 0xCC
#define ALARM_SEARCH 0xEC
/* @} */

/** \defgroup DS18S20FuncCmd DS18S20 Commands */
/* @{ */
#define CONVERT_T 0x44
#define WRITE_SCRATCHPAD 0x4E
#define READ_SCRATCHPAD 0xBE
#define COPY_SCRATCHPAD 0x48
#define RECALL_E2 0xB8
#define READ_POWER_SUPPLAY 0xB4
/* @} */

/** \defgroup DS18B20Conf DS18B20 Config Register */
/* @{ */
#define CONF_RES_9b 0x1F
#define CONF_RES_10b 0x3F
#define CONF_RES_11b 0x5F
#define CONF_RES_12b 0x7F
/* @} */

/**
 * @brief Represents the DS18S20 sensor including the 1-Wire bus configuration (MCU PORT and PIN
 * selected for DS18S20 connection).
 */
typedef struct SDS18S20 
{
		volatile uint8_t *DS18x20_PORT; /**< pointer to the MCU PORT to which DS18S20 sensor is connected.*/
		uint8_t DS18x20_PIN; /**< PIN from the MCU PORT connected to the DS18S20.*/	
		uint8_t serialNumber[8];	/**< buffer for the DS18S20 serial number (its address on 1-wire bus)*/
		uint8_t scratchpad[9];	/**< DS18S20 scratchpad */
		TSensorModel SensorModel;	/**< Sensor Model */ 
}TSDS18S20;

/**
 * @brief Defines the sensor model.
 */
typedef enum ESensorModel
{
	DS18S20Sensor = 0, /**< Define the sensor as DS18S20 */
	DS18B20Sensor /**< Define the sensor as DS18B20 */
} TSensorModel;

/** \defgroup libMacros DS18S20 library macros */
/* @{ */
/**
 * @def DS18x20_SendCommand(DS18S20,command) OWWriteByte(DS18S20,command)
 * Sends a command to the DS18S20 sensor via 1-Wire bus interface.
 *
 */
#define DS18x20_SendCommand(DS18S20,command) OWWriteByte(DS18S20,command)
/* @} */

/** \defgroup OneWire Implementation of 1-Wire Interface */
/* @{ */
/**Send one bit via 1-Wire interface to DS18S20 sensor.
 * 
 * @param[in] pDS18S20 pointer to the structure that represent DS18S20
 * @param[in] bit a bit value to be send
 * @return void
 *
*/
void OWWriteBit(TSDS18S20 *pDS18S20, uint8_t bit);

/**Send one byte via 1-Wire interface to DS18S20 sensor.
 * 
 * @param[in] pDS18S20 pointer to the structure that represent DS18S20
 * @param[in] value a byte value to be send
 * @return void
 *
*/
void OWWriteByte(TSDS18S20 *pDS18S20, uint8_t value);

/**Read a bit from the 1-Wire interface.
 * 
 * @param[in] pDS18S20 pointer to the structure that represent DS18S20
 * @return uint8_t value read from the 1-Wire interface
 *
*/
uint8_t OWReadBit(TSDS18S20 *pDS18S20);

/**Read a byte from the 1-Wire interface.
 * 
 * @param[in] pDS18S20 pointer to the structure that represent DS18S20
 * @return uint8_t byte read from the 1-Wire interface
 *
*/
uint8_t OWReadByte(TSDS18S20 *pDS18S20);

/**Used for initialization of DS18S20 sensor before data exchange.
 * 
 * @param[in] pDS18S20 pointer to the structure that represent DS18S20
 * @return uint8_t Returns 0 for success, 1 for failed initialization attempt.
 *
*/
uint8_t OWReset(TSDS18S20 *pDS18S20);
/* @} */

/** \defgroup DS18S20func DS18S20 functions */
/* @{ */
	
/**
 * @brief  Init function for DS18S20.
 * The function prepares the 1-Wire bus and runs the sensors initialization procedure.
 * 
 * @param[in,out] pDS18S20 pointer to the structure that represent DS18S20
 * @param[in] DS18x20_PORT is a pointer to the MCU PORT used for the 1-Wire bus
 * @param[in] DS18x20_PIN MCU PIN from DS18x20_PORT connected to the DS18S20 sensor
 * @return uint8_t. 1 for success init, 0 for failure.
 */	
uint8_t DS18x20_Init(TSDS18S20 *pDS18S20,volatile uint8_t *DS18x20_PORT,uint8_t DS18x20_PIN);
	
/**
 * @brief Reads DS18S20 64-bit ROM code without using the Search ROM procedure.
 * 
 * @param[in,out] pDS18S20 pointer to the structure that represent DS18S20
 * @return uint8_t 1 for valid data transfer, 0 for CRC error detected.
 */	
uint8_t DS18x20_ReadROM(TSDS18S20 *pDS18S20);

/**
 * @brief This functions initiates a single temperature conversion.
 * 
 * @param[in,out] pDS18S20 pointer to the structure that represent DS18S20
 * @return void
 */	
void DS18x20_MeasureTemperature(TSDS18S20 *pDS18S20);

/**
 * @brief Read the contents of DS18S20 scratchpad.
 * 
 * @param[in,out] pDS18S20 pointer to the structure that represent DS18S20
 * @return uint8_t 0 if CRC error is detected, 1 for successful scratchpad read.
 */	
uint8_t DS18x20_ReadScratchPad(TSDS18S20 *pDS18S20);

/**
 * @brief Returns the power supply type based on the respond from the sensor on Read Power Supply 
 * function command.
 * 
 * @param[in] pDS18S20 pointer to the structure that represent DS18S20
 * @return uint8_t 0 for the parasite powered, 1 for externally powered sensor.
 */	
uint8_t DS18x20_PowerSupplyType(TSDS18S20 *pDS18S20);

/**
 * @brief Writes 2 bytes of data to the SA18S20 scratchpad (TH and TL registers).
 * 
 * @param[in] pDS18S20 pointer to the structure that represent DS18S20
 * @param[in] TH value of the TH register
 * @param[in] TL value of the TL register 
 * @return void
 */	
void DS18x20_WriteScratchpad(TSDS18S20 *pDS18S20, uint8_t TH, uint8_t TL);

/**
 * @brief Copies the contents of the scratchpad TH and TL registers (bytes 2 and 3) to EEPROM.
 *
 * @param [in] pDS18S20 pointer to the structure that represent DS18S20
 * @return void
 *
 */
void DS18x20_CopyScratchpad(TSDS18S20 *pDS18S20);

/**
 * @brief Recalls the alarm trigger values (TH and TL) from EEPROM.
 *
 * @param [in] pDS18S20 pointer to the structure that represent DS18S20
 * @return void
 *
 */
void DS18x20_RECALL_E2(TSDS18S20 *pDS18S20);
/* @} */

#endif /* DS18S20_H_ */
