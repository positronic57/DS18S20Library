#Makefile for bor building the examples distributed 
#with DS18S20Library.

#AVR Microcontroller
MCU = atmega32

#CPU frequency  (must be the same as the value used while building DS18S20Library).
CPU_FREQ = 16000000L

#Header files path (Update this with the library installation folder).
HEADERS_PATH = /usr/lib/avr/include

#Library path
LIBRARY_PATH = /usr/lib/avr/lib/atmega32

#List of source files
SOURCES += DS18B20example1.c DS18B20example2.c DS18S20example1.c UART.c

#List of object files
OBJS += DS18B20example1.o DS18B20example2.o DS18S20example1.o UART.o

#Define C compiler flags
override CFLAGS = -Wall -I$(HEADERS_PATH) -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=$(MCU) -DF_CPU=$(CPU_FREQ)

.PHONY: DS18B20example1
.PHONY: DS18B20example2
.PHONY: DS18S20example1

DS18B20example1: DS18B20example1.hex
DS18B20example2: DS18B20example2.hex
DS18S20example1: DS18S20example1.hex

all: DS18B20example1 DS18B20example2 DS18S20example1

#Compile all of the source files
DS18B20example1.o: DS18B20example1.c 
	@echo Building file: $<
	avr-gcc $(CFLAGS) -c $< -o $@
	@echo

DS18B20example2.o: DS18B20example2.c 
	@echo Building file: $<
	avr-gcc $(CFLAGS) -c $< -o $@
	@echo

DS18S20example1.o: DS18S20example1.c 
	@echo Building file: $<
	avr-gcc $(CFLAGS) -c $< -o $@
	@echo

UART.o: UART.c 
	@echo Building file: $<
	avr-gcc $(CFLAGS) -c $< -o $@
	@echo

#Generate DS18B20example ELF file from the object files using linker
DS18B20example1.elf: DS18B20example1.o UART.o
	@echo Building target file: $@. 
	avr-gcc -Wl,-Map,DS18B20example1.map -mmcu=$(MCU) -o $@ $^ -L$(LIBRARY_PATH) -lDS18S20 -lm
	@echo 
	@echo $@ memory footprint:
	avr-size --format=avr --mcu=atmega32 $@

#Generate DS18B20Example2 ELF file from the object files using linker
DS18B20example2.elf: DS18B20example2.o UART.o
	@echo Building target file: $@. 
	avr-gcc -Wl,-Map,DS18B20example2.map -mmcu=$(MCU) -o $@ $^ -L$(LIBRARY_PATH) -lDS18S20 
	@echo 
	@echo $@ memory footprint:
	avr-size --format=avr --mcu=atmega32 $@

#Generate DS18S20Example1 ELF file from the object files using linker
DS18S20example1.elf: DS18S20example1.o UART.o
	@echo Building target file: $@. 
	avr-gcc -Wl,-Map,DS18S20example1.map -mmcu=$(MCU) -o $@ $^ -L$(LIBRARY_PATH) -lDS18S20 
	@echo 
	@echo $@ memory footprint:
	avr-size --format=avr --mcu=atmega32 $@

#Generate HEX files
DS18B20example1.hex: DS18B20example1.elf
	@echo Creating object file in ihex format.
	avr-objcopy -j .text -j .data -O ihex $< $@
	@echo

DS18B20example2.hex: DS18B20example2.elf
	@echo Creating object file in ihex format.
	avr-objcopy -j .text -j .data -O ihex $< $@
	@echo

DS18S20example1.hex: DS18S20example1.elf
	@echo Creating object file in ihex format.
	avr-objcopy -j .text -j .data -O ihex $< $@
	@echo

.PHONY: clean

clean:
	rm -f *.o *.elf *.hex *.map
 
