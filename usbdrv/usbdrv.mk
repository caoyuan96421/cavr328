
USBDRV = usbdrv

OBJECTS += $(USBDRV)/usbdrv.o
OBJECTS += $(USBDRV)/oddebug.o
OBJECTS += $(USBDRV)/usbdrvasm.o

OBJ-BOOTLOADER += $(USBDRV)/usbdrv.o
OBJ-BOOTLOADER += $(USBDRV)/oddebug.o
OBJ-BOOTLOADER += $(USBDRV)/usbdrvasm.o

