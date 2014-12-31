import usb.core
import usb.util
import sys

USB_READ_REQUEST = 0xC0
USB_WRITE_REQUEST = 0x40
USB_WRITE_DATA = 0x02

vendor_id = 0x16c0
product_id = 0x03e8

class SwitchMatrix(object):
    def __init__(self):
        self.connect()
    def connect(self):
        self.device = usb.core.find(idVendor = vendor_id, idProduct = product_id)
        if self.device is None:
            return
        self.device.set_configuration()
    def write(self, data):
        self.device.ctrl_transfer(USB_WRITE_REQUEST, USB_WRITE_DATA, data, 0, None)

#s = SwitchMatrix()
#s.connect()
#s.write(0x23)
