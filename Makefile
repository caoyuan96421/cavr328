# Name: Makefile
# Project: hid-mouse example
# Author: Christian Starkjohann
# Creation Date: 2008-04-07
# Tabsize: 4
# Copyright: (c) 2008 by OBJECTIVE DEVELOPMENT Software GmbH
# License: GNU GPL v2 (see License.txt), GNU GPL v3 or proprietary (CommercialLicense.txt)

PROJECT = avr
DEVICE  = atmega328
F_CPU   = 16000000L	# in Hz

PROGRAMMER = avrispmk2
INTERFACE = ISP
PROGFREQ = 1mhz


CFLAGS  = -DDEBUG_LEVEL=0
OBJECTS =
INC = -Iinclude -Isrc
DEF = 
LIB = -lm


# Include source files in other directories
include avr/avr.mk src/src.mk 

TOOLCHAIN = avr
GCC = $(TOOLCHAIN)-gcc
OBJCOPY = $(TOOLCHAIN)-objcopy
OBJDUMP = $(TOOLCHAIN)-objdump
SIZE = $(TOOLCHAIN)-size
GCC_OPTS = -Wall -Os
COMPILE = $(GCC) $(GCC_OPTS) -DF_CPU=$(F_CPU) $(DEF) $(INC) $(CFLAGS) -mmcu=$(DEVICE)

# Default rule
all: bin/$(PROJECT).elf bin/$(PROJECT).hex bin/$(PROJECT).dump
	@echo Done.

# Generic rule for compiling C files:
.c.o:
	$(COMPILE) -c $< -o $@

# Generic rule for assembling Assembler source files:
.S.o:
	$(COMPILE) -x assembler-with-cpp -c $< -o $@
# "-x assembler-with-cpp" should not be necessary since this is the default
# file type for the .S (with capital S) extension. However, upper case
# characters are not always preserved on Windows. To ensure WinAVR
# compatibility define the file type manually.

bin/$(PROJECT).elf: $(OBJECTS)
	@echo Compile for .elf file
	$(COMPILE) $(LIB) -Wl,-Map,bin/$(PROJECT).map -o bin/$(PROJECT).elf $(OBJECTS)

bin/$(PROJECT).hex: bin/$(PROJECT).elf
	rm -f bin/$(PROJECT).hex
	$(OBJCOPY) -j .text -j .data -O ihex bin/$(PROJECT).elf bin/$(PROJECT).hex
	$(SIZE) bin/$(PROJECT).hex

bin/$(PROJECT).dump: bin/$(PROJECT).elf
	$(OBJDUMP) -d bin/$(PROJECT).elf > bin/$(PROJECT).dump
	
flash: bin/$(PROJECT).hex
	atprogram -t $(PROGRAMMER) -i $(INTERFACE) -d $(DEVICE) -cl $(PROGFREQ) \
		program --verify --format hex --flash -c -f bin/$(PROJECT).hex
	
	
clean:
	@echo $(OBJECTS)
	rm -fR $(OBJECTS)
	rm -fR bin/*.hex
	rm -fR bin/*.elf
	rm -fR bin/*.dump
	rm -fR bin/*.map
