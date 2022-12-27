#########################################################################
#
# Description: This file defines the build of DS18S20library project as 
# a static library for AVR8 MCUs with CMake and avr-gcc.
#
# Created: 26-Dec-2022
# Author: Goce Boshkovski
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License.
#
#########################################################################

cmake_minimum_required(VERSION 3.1)

# Define the project name
project(libDS18S20)

# Show the compiler commands during the build 
set(CMAKE_VERBOSE_MAKEFILE ON)

include(GNUInstallDirs)

# Define the variables/command line arguments for target MCU and MCU clock frequency
set(MCU "atmega32" CACHE STRING "Target micro-controller with default value atmega32")
set(CPU_FREQ "16000000L" CACHE STRING "MCU CPU frequency")

# Define the extra include directories here
#include_directories(/usr/local/include/avr8)

# Add mmcu as compiler option
add_compile_options(-mmcu=${MCU})

# Define linker options
add_link_options(-mmcu=${MCU} -Wl,-Map=${PROJECT_NAME}.map)

# Define the F_CPU macro required for some avr-libc functions
add_definitions(-DF_CPU=${CPU_FREQ})

# Define the list of source files.
set (SOURCE_LIST
ds18S20.c
)

# Build the project as static library
add_library(DS18S20 STATIC ${SOURCE_LIST})

# Installation steps. They will be executed in the same order as listed/defined.
# Create the target folder for installation
install(DIRECTORY DESTINATION ${CMAKE_INSTALL_LIBDIR}/avr8/${MCU})

# Copy the build artifact in the installation folder
install(TARGETS DS18S20
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/avr8/${MCU}
)

# Copy the header file to the destination for headers
install(FILES ds18S20.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/avr8)
