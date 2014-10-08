
USBDRV = usbdrv

OBJECTS += $(USBDRV)/usbdrv.o
OBJECTS += $(USBDRV)/usbdrvasm.o

OBJ-BOOTLOADER += $(USBDRV)/usbdrv.o
OBJ-BOOTLOADER += $(USBDRV)/usbdrvasm.o

