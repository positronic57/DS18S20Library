#Makefile that builds DS18x20 library for 8-bit AVR MCU.

#Project name
LIBRARY_NAME = libDS18S20

#AVR Microcontroller
ifdef mcu
        MCU = $(mcu)
else
        MCU = atmega32
endif

#CPU frequency
ifndef cpu_freq
        CPU_FREQ = 16000000L
else
        CPU_FREQ = $(cpu_freq)
endif


#Source files
SOURCES += ds18S20.c

#List of object files
OBJECTS += $(SOURCES:.c=.o)

#Library installation folder
PREFIX = /usr/local

#Define C compiler flags
override CFLAGS = -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=$(MCU) -DF_CPU=$(CPU_FREQ)
#Define avr-ar options
override ARAGS = rcs


all: $(LIBRARY_NAME).a

$(SOURCES): ds18S20.h

#Compile all of the source files
$(OBJECTS): $(SOURCES)
	@echo Compiling file: $<
	avr-gcc $(CFLAGS) -c $^ -o $@
	@echo

#Builing the library
$(LIBRARY_NAME).a: $(OBJECTS)
	@echo Building target file: $@. 
	avr-ar $(ARAGS) $@ $^

#Install the library
install: $(LIBRARY_NAME).a
	if [ ! -d $(PREFIX)/lib/avr8/$(MCU) ]; then mkdir $(PREFIX)/lib/avr8/$(MCU); fi
	if [ ! -d $(PREFIX)/include/avr8 ]; then mkdir -p $(PREFIX)/include/avr8; fi
	install -m 0644 $(LIBRARY_NAME).a $(PREFIX)/lib/avr8/$(MCU)
	install -m 0644 ds18S20.h $(PREFIX)/include/avr8

#Uninstall the library	
.PHONY: uninstall
uninstall:
	rm $(PREFIX)/lib/avr8/$(MCU)/$(LIBRARY_NAME).a
	rm $(PREFIX)/include/avr8/ds18S20.h

.PHONY: clean
clean:
	rm -rf $(LIBRARY_NAME).a $(OBJECTS)

