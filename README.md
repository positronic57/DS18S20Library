## DS18S20/DS18B20 sensor C library

### Short Description

DS18S20library is AVR C library for connecting DS18S20 and DS18B20 temperature sensors to 8-bit AVR microcontrollers.
The library implements 1-Wire interface for communication with DS18S20/DS18B20.

Originally it was written for DS18S20 and starting from version 0.4 the library fully supports
DS18B20 sensor model as well.


### Supported MCU Hardware

The library is compatible with every 8-bit AVR microcontroller.

### DS18x20 Support and Implemented Functions

The following sensor models are supported by the library:

- DS18S20;
- DS18B20.

The library can be used only in cases when DS18x20 sensor is powered by external power supply.
Although the library includes a function that can determine the power supply type, this information is not taken in 
consideration when the library communicates with the sensor via the 1-Wire bus.

Current version supports 1-Wire bus with only one device attached to it.
SEARCH_ROM is not implemented in the library so it cannot query the bus for connected devices/sensors.

Implemented DS18x20 ROM Commands:

- DS18S20/DS18B20 sensor initialization;
- READ ROM command for fetching the ROM address of the sensor;
- SKIP ROM command so the library can speak to the sensor directly without using sensors 1-Wire bus address.

Implemented DS18x20 Function Commands:

- Convert T command that initiates a single temperature conversion;
- READ SCRATCHPAD command that allows the master to read the contents of the sensor memory;
- READ POWER SUPPLY command to determine power supply type of DS18S20/DS18B20;
- WRITE SCRATCHPAD command that allows the master to write 2 bytes of data to the scratchpad of DS18S20/DS18B20;
- COPY SCRATCHPAD command that copies the contents of the scratchpad TH and TL registers (bytes 2 and 3) to EEPROM;
- RECALL E2 recalls the alarm trigger values (TH and TL) from EEPROM of the sensor.

1-Wire interface

The underline implementation of 1-Wire interface relies on timer functions that are part of AVR-libc and defined in util/delay.h
header. Those functions require the CPU freq to be defined via F_CPU macro. This value can be set globally during the compiling time
using -DF_CPU=<value>L switch. If this macro is not defined, the library will assign a default value of 4MHz.

### Installation

Is not mandatory to do any installation, the source files can be imported in the target AVR8 project and used directly.
Anyhow, the code is distributed with make and cmake configuration files for building the code as a static library for linking with other projects.

#### Creating a static library with make on Linux

Requirements: avr-gcc and avr-libc installed on the system.

Execute the make command with the arguments for MCU type,  CPU frequency and installation folder:

```
#make MCU=attiny2313 CPU_FREQ=20000000L prefix=/home/username
```
In this exаmple, a static library will be created for ATtiny2313 MCU with 20MHz clock and the installation folder is set to /home/username/lib/avr8/attiny2313
If not provided, the default values of those arguments will be used:
- MCU: atmega32
- CPU_FREQ: 16MHz
- installation prefix: /usr/local

After the build, Install the static library with:

```
#make install
```
or
```
#sudo make install
```
 if writing to the destination folder requires root privileges.

#### Creating a static library with CMake on Linux

Requirements: cmake, avr-gcc and avr-libc installed on the system.

Create a build folder within the project folder and change to that folder:
```
#mkdir build && cd build
```
Create the build instructions with cmake:
```
#cmake  -DCMAKE_TOOLCHAIN_FILE=../avr8_toolchain.cmake -DMCU=attiny2313 -DCPU_FREQ=20000000L -DCMAKE_INSTALL_PREFIX=/home/username/lib ..
```
which will prepare the build for ATtiny2313 microcontroller with working frequency of 20MHz and the result of the build will be installed in /home/username/lib.
If not provided, the default installation folder will be: /usr/local/lib/avr8/\<mcu_name\> for the library file and /usr/local/include/avr8 for the header file.
Build the static library with:

```
#make
```
Installation is done the same way as with make tool (see the section above).


### Usage in code

The library use DS18x20_Init function to define the 1-Wire bus and initialize the sensor. The function detects the sensor
model (DS18B20 or DS18S20) during the initialization process.

For bus definition, the init function requires two arguments:

- the address of MCU PORT used for 1-Wire bus;
- MCU pin that is connected to the DQ pin of DS18S20/DS18B20.

The first argument of the init function is a pointer to the structure that represents the sensor. 

For example, in case the sensor is connected to pin 5 of port D, the init function will have the following arguments:

	TSDS18x20 DS18x20;

	DS18x20_Init(&DS18x20, &PORTD, PD5);

After the initialization step, the rest of the library functions/macros can be used to
get the address of the sensor, start a temperature conversion, define temperature resolution in case of DS18B20 sensor etc.

For example:

	DS18x20_MeasureTemperature(&DS18x20);

initiates a temperature conversion and fetches the sensor memory with the temperature reading. The only argument of the function is 
the same pointer as the one in DS18x20_Init() function.

See the documentation section for further details.

Tested on Atmega32 and Atiny2313 microcontrollers.

### Translations
- [Macedonian version of this text](translations/README_mk.md)

## WARNING:
The source is provided as is without any warranty. Use it on your own risk!
The author does not take any responsibility for the damage caused while using this software.

## DISCLAIMER: 
The code is a result of a hobby work and the author is not affiliated with any of the hardware/components/boards/tools manufacturers/creators mentioned in the code, documentation or the description of this project. All trademarks are the property of the respective owners.

