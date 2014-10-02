# Name: Makefile
# Project: hid-m
# Author: Yuan Cao
# Creation Date: 2014-09-30
# Tabsize: 4

PROJECT = avr
OUTPUT = bin
DEVICE  = atmega328
F_CPU   = 16000000L	# in Hz

PROGRAMMER = avrispmk2
INTERFACE = ISP
PROGFREQ = 2mhz
PROG_FUSE = 1

ifeq ($(PROG_FUSE),1)
	COPYFUSE = -j .fuse
	PROGFUSE = -fs
else
	COPYFUSE = 
	PROGFUSE = 
endif


CFLAGS  = -DDEBUG 
OBJECTS =
INC = -I../include -I../src -I../usbdrv
DEF = 
LIB = -Wl,-lm
LINKFLAG = -Wl,-Map,$(OUTPUT)/$(PROJECT).map -Wl,--gc-sections
GCC_OPTS = -Wall -Os -g2


# Include source files in other directories
include avr/avr.mk src/src.mk usbdrv/usbdrv.mk

OBJECTS_ABS := $(OBJECTS:%.o=bin/%.o)
TOOLCHAIN = avr
CC = $(TOOLCHAIN)-gcc
OBJCOPY = $(TOOLCHAIN)-objcopy
OBJDUMP = $(TOOLCHAIN)-objdump
SIZE = $(TOOLCHAIN)-size

COMPILE = $(CC) $(GCC_OPTS) -DF_CPU=$(F_CPU) $(DEF) $(INC) $(CFLAGS) -mmcu=$(DEVICE)  
LINK = $(CC) -mmcu=$(DEVICE) $(LIB) $(LINKFLAG)

# Default rule
all: $(OUTPUT)/$(PROJECT).elf $(OUTPUT)/$(PROJECT).hex $(OUTPUT)/$(PROJECT).dump
	@echo Done.

.SECONDARY : $(OUTPUT)/$(PROJECT).elf $(OBJECTS_ABS)
# Generic rule for compiling C files:
$(OUTPUT)/%.o: %.c
ifeq ("$(wildcard,$(@D))","")
	-mkdir $(@D)
endif
	cd $(OUTPUT) && $(COMPILE) -c ../$< -o ../$@ 

# Generic rule for assembling Assembler source files:
$(OUTPUT)/%.o: %.S
	cd $(OUTPUT) && $(COMPILE) -x assembler-with-cpp -c ../$< -o ../$@ 
# "-x assembler-with-cpp" should not be necessary since this is the default
# file type for the .S (with capital S) extension. However, upper case
# characters are not always preserved on Windows. To ensure WinAVR
# compatibility define the file type manually.

%.elf: $(OBJECTS_ABS)
	@echo Compile for .elf file
	$(LINK) -o $@ $(OBJECTS_ABS) 

%.hex: %.elf
	rm -f $@
	$(OBJCOPY) -j .text -j .data $(COPYFUSE) -O ihex $< $@
	$(SIZE) $@

%.dump: %.elf
	$(OBJDUMP) -h -S $< > $@
	
flash: $(OUTPUT)/$(PROJECT).elf
	atprogram -t $(PROGRAMMER) -i $(INTERFACE) -d $(DEVICE) -cl $(PROGFREQ) \
		program --verify --format elf --flash -c $(PROGFUSE) -f $(OUTPUT)/$(PROJECT).elf
	
bootloader: LINKFLAG += -Wl,--section-start,.text=0x7000
# 0x7000 = 0x3800 (words), start of bootloader region
bootloader: DEF += -DBOOTLOADER
bootloader: $(OUTPUT)/$(PROJECT).elf $(OUTPUT)/$(PROJECT).hex $(OUTPUT)/$(PROJECT).dump
		
.PHONY clean:
	cd $(OUTPUT) && rm -f $(OBJECTS) $(OBJECTS:%.o=%.d)
	rm -f $(OUTPUT)/*.hex
	rm -f $(OUTPUT)/*.elf
	rm -f $(OUTPUT)/*.dump
	rm -f $(OUTPUT)/*.map
