import usb.core
import usb.util
import sys

USB_READ_REQUEST = 0xC0
USB_WRITE_REQUEST = 0x40
USB_READ_DATA = 0x01
USB_WRITE_DATA = 0x02

vendor_id = 0x16c0
product_id = 0x03e8

class FPGADebugger(object):
    def __init__(self):
        self.connect()
    def connect(self):
        self.device = usb.core.find(idVendor = vendor_id, idProduct = product_id)
        if self.device is None:
            return
        self.device.set_configuration()
    def read(self):
        data = self.device.ctrl_transfer(USB_READ_REQUEST, USB_READ_DATA, 0, 0, 2)
        data = data[1] << 8 | data[0]
        return data
    def write(self, data):
        self.device.ctrl_transfer(USB_WRITE_REQUEST, USB_WRITE_DATA, data, 0, None)

#fpga = FPGADebugger()
#print('0x%04x' % fpga.read())
#fpga.write(fpga.read())
