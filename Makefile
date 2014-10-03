# Name: Makefile
# Project: hid-m
# Author: Yuan Cao
# Creation Date: 2014-09-30
# Tabsize: 4


.SECONDEXPANSION:
PROJECT = avr
OUTPUT = bin
DEVICE  = atmega328
F_CPU   = 16000000L
OBJECT_$(PROJECT) = OBJECTS

OBJECT_avr-bootloader = OBJ-BOOTLOADER

PROGRAMMER = avrispmk2
INTERFACE = ISP
<<<<<<< HEAD
PROGFREQ = 1mhz
=======
PROGFREQ = 2mhz
PROG_FUSE = 1

ifeq ($(PROG_FUSE),1)
	COPYFUSE = -j .fuse
	PROGFUSE = -fs
else
	COPYFUSE = 
	PROGFUSE = 
endif
>>>>>>> origin/bootloader


CFLAGS  = -DDEBUG 
INC = -I../usbdrv
DEF = 
LIB = -Wl,-lm
LDFLAGS = -Wl,-Map,$(basename $@).map -Wl,--gc-sections
GCC_OPTS = -Wall -Os -g2


# Include source files in other directories
include avr/avr.mk src/src.mk usbdrv/usbdrv.mk bootloader/bootloader.mk


TOOLCHAIN = avr
CC = $(TOOLCHAIN)-gcc
OBJCOPY = $(TOOLCHAIN)-objcopy
OBJDUMP = $(TOOLCHAIN)-objdump
SIZE = $(TOOLCHAIN)-size

COMPILE = $(CC) $(GCC_OPTS) -DF_CPU=$(F_CPU) $(DEF) $(INC) $(CFLAGS) -mmcu=$(DEVICE)  
LD = $(CC) -mmcu=$(DEVICE) $(LIB) $(LDFLAGS)

.SECONDARY : $(OUTPUT)/$(PROJECT).elf $(OBJECTS:%.o=$(OUTPUT)/%.o)

# Default rule
all: INC += -I../src -I../include 
all: $(OUTPUT)/$(PROJECT).elf $(OUTPUT)/$(PROJECT).hex $(OUTPUT)/$(PROJECT).dump
	@echo Done.

bootloader: LDFLAGS += -Wl,--section-start,.text=0x7000
bootloader: INC += -I../bootloader
bootloader: $(OUTPUT)/$(PROJECT)-bootloader.elf $(OUTPUT)/$(PROJECT)-bootloader.hex $(OUTPUT)/$(PROJECT)-bootloader.dump
	
# Generic rule for compiling C files:
$(OUTPUT)/%.o: %.c
	@-mkdir $(@D)
	cd $(OUTPUT) && $(COMPILE) -c ../$< -o ../$@ 

# Generic rule for assembling Assembler source files:
$(OUTPUT)/%.o: %.S
	@-mkdir $(@D)
	cd $(OUTPUT) && $(COMPILE) -x assembler-with-cpp -c ../$< -o ../$@ 
# "-x assembler-with-cpp" should not be necessary since this is the default
# file type for the .S (with capital S) extension. However, upper case
# characters are not always preserved on Windows. To ensure WinAVR
# compatibility define the file type manually.

%.elf: $$(addprefix $(OUTPUT)/,$$($$(OBJECT_$$(basename $$(notdir $$@)))))
	@echo Compile for $*.elf file, objects $^
	$(LD) -o $@ $^

%.hex: %.elf
	rm -f $@
	$(OBJCOPY) -j .text -j .data $(COPYFUSE) -O ihex $< $@
	$(SIZE) $@

%.dump: %.elf
	$(OBJDUMP) -h -S $< > $@
	
flash: all $(OUTPUT)/$(PROJECT).elf
	atprogram -t $(PROGRAMMER) -i $(INTERFACE) -d $(DEVICE) -cl $(PROGFREQ) \
		program --verify --format elf --flash -c $(PROGFUSE) -f $(OUTPUT)/$(PROJECT).elf
		
flash-bootloader: bootloader $(OUTPUT)/$(PROJECT)-bootloader.elf
	atprogram -t $(PROGRAMMER) -i $(INTERFACE) -d $(DEVICE) -cl $(PROGFREQ) \
		program --verify --format elf --flash -c $(PROGFUSE) -f $(OUTPUT)/$(PROJECT)-bootloader.elf

		
.PHONY clean:
	cd $(OUTPUT) && rm -f $(OBJECTS) $(OBJECTS:%.o=%.d)
	rm -f $(OUTPUT)/*.*
