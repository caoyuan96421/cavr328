# Name: Makefile
# Project: hid-m
# Author: Yuan Cao
# Creation Date: 2014-09-30
# Tabsize: 4


.SECONDEXPANSION:
PROJECT = avr
DEVICE  = atmega328
F_CPU   = 16000000L
OBJECT_$(PROJECT) = OBJECTS

OBJECT_avr-bootloader = OBJ-BOOTLOADER

PROGRAMMER = avrispmk2
INTERFACE = ISP
PROGFREQ = 1mhz
PROGFREQ = 2mhz
PROG_FUSE = 1

ifeq ($(PROG_FUSE),1)
	COPYFUSE = -j .fuse
	PROGFUSE = -fs
else
	COPYFUSE = 
	PROGFUSE = 
endif


<<<<<<< HEAD
CFLAGS  = -DDEBUG 
INC = -I../usbdrv
DEF = 
LIB = -Wl,-lm
LDFLAGS = -Wl,-Map,$(basename $@).map -Wl,--gc-sections
GCC_OPTS = -Wall -Os -g2
=======
CFLAGS  = -DDEBUG_LEVEL=0
OBJECTS =
INC = -Iinclude -Isrc -Iusbdrv
DEF = 
LIB = -lm
LINKFLAG = -Wl,-Map,bin/$(PROJECT).map
>>>>>>> parent of 906c14e... Bootloader working


# Include source files in other directories
include avr/avr.mk src/src.mk usbdrv/usbdrv.mk bootloader/bootloader.mk


TOOLCHAIN = avr
GCC = $(TOOLCHAIN)-gcc
OBJCOPY = $(TOOLCHAIN)-objcopy
OBJDUMP = $(TOOLCHAIN)-objdump
SIZE = $(TOOLCHAIN)-size
<<<<<<< HEAD

COMPILE = $(CC) $(GCC_OPTS) -DF_CPU=$(F_CPU) $(DEF) $(INC) $(CFLAGS) -mmcu=$(DEVICE)  
LD = $(CC) -mmcu=$(DEVICE) $(LIB) $(LDFLAGS)

.SECONDARY : $(OUTPUT)/$(PROJECT).elf

# Default rule
all: INC += -I../src -I../include 
all: $(OUTPUT)/$(PROJECT).elf $(OUTPUT)/$(PROJECT).hex $(OUTPUT)/$(PROJECT).dump $(addprefix $(OUTPUT)/,$OBJECTS)
	@echo Done.

bootloader: LDFLAGS += -Wl,--section-start,.text=0x7000
bootloader: INC += -I../bootloader
bootloader: $(OUTPUT)/$(PROJECT)-bootloader.elf $(OUTPUT)/$(PROJECT)-bootloader.hex $(OUTPUT)/$(PROJECT)-bootloader.dump
	
# Generic rule for compiling C files:
$(OUTPUT)/%.o: %.c
	@-mkdir -p $(@D)
	cd $(OUTPUT) && $(COMPILE) -c ../$< -M -MF ../$(@:%.o=%.d) -o ../$@

# Generic rule for assembling Assembler source files:
$(OUTPUT)/%.o: %.S
	@-mkdir -p $(@D)
	cd $(OUTPUT) && $(COMPILE) -x assembler-with-cpp -c ../$< -o ../$@
=======
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
>>>>>>> parent of 906c14e... Bootloader working
# "-x assembler-with-cpp" should not be necessary since this is the default
# file type for the .S (with capital S) extension. However, upper case
# characters are not always preserved on Windows. To ensure WinAVR
# compatibility define the file type manually.

<<<<<<< HEAD
%.elf: $$(addprefix $(OUTPUT)/,$$($$(OBJECT_$$(basename $$(notdir $$@)))))
	@echo Compile for $*.elf file, objects $^
	$(LD) -o $@ $^
=======
bin/$(PROJECT).elf: $(OBJECTS)
	@echo Compile for .elf file
	$(COMPILE) $(LIB) $(LINKFLAG) -o bin/$(PROJECT).elf $(OBJECTS)
>>>>>>> parent of 906c14e... Bootloader working

bin/$(PROJECT).hex: bin/$(PROJECT).elf
	rm -f bin/$(PROJECT).hex
	$(OBJCOPY) -j .text -j .data $(COPYFUSE) -O ihex bin/$(PROJECT).elf bin/$(PROJECT).hex
	$(SIZE) bin/$(PROJECT).hex

bin/$(PROJECT).dump: bin/$(PROJECT).elf
	$(OBJDUMP) -d bin/$(PROJECT).elf > bin/$(PROJECT).dump
	
<<<<<<< HEAD
flash: all $(OUTPUT)/$(PROJECT).elf
	atprogram -t $(PROGRAMMER) -i $(INTERFACE) -d $(DEVICE) -cl $(PROGFREQ) \
		program --verify --format elf --flash -c $(PROGFUSE) -f $(OUTPUT)/$(PROJECT).elf
		
flash-bootloader: bootloader $(OUTPUT)/$(PROJECT)-bootloader.elf
	atprogram -t $(PROGRAMMER) -i $(INTERFACE) -d $(DEVICE) -cl $(PROGFREQ) \
		program --verify --format elf --flash -c $(PROGFUSE) -f $(OUTPUT)/$(PROJECT)-bootloader.elf

		
.PHONY clean:
	rm -fR $(OUTPUT)
=======
flash: bin/$(PROJECT).elf
	atprogram -t $(PROGRAMMER) -i $(INTERFACE) -d $(DEVICE) -cl $(PROGFREQ) \
		program --verify --format elf --flash -c $(PROGFUSE) -f bin/$(PROJECT).elf
	
bootloader: LINKFLAG += -Wl,--section-start,.text=0x7000
# 0x7000 = 0x3800 (words), start of bootloader region
bootloader: DEF += -DBOOTLOADER
bootloader: bin/$(PROJECT).hex bin/$(PROJECT).dump
		
clean:
	@echo $(OBJECTS)
	rm -fR $(OBJECTS)
	rm -fR bin/*.hex
	rm -fR bin/*.elf
	rm -fR bin/*.dump
	rm -fR bin/*.map
>>>>>>> parent of 906c14e... Bootloader working
