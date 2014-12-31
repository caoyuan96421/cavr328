
import tkinter as tk
import time
from threading import Thread
from SwitchMatrix import *

class Application(tk.Tk):
    def __init__(self, master=None):
        tk.Tk.__init__(self,master)
        self.grid()
        self.createWidgets()
        self.device = None
        self.thread = Thread(target = self.run, name = 'SwitchMatrix Daemon')
        self.kill = False
        self.thread.start()
    def destroy(self):
        self.kill = True
        self.thread.join()
        tk.Tk.destroy(self)

    def createWidgets(self):
        
        self.connectButton = tk.Button(self, text='Connect', command = self.connect)
        self.connectButton.grid()
        self.quitButton = tk.Button(self, text='Quit', command = self.destroy)
        self.quitButton.grid(row=0,column=1)

        tk.Label(self, text='Write: 0x').grid()

        self.writeDataVar = tk.StringVar(value='00')
        self.writeDataVar.trace("w", self.genHex)
        self.writeData = tk.Entry(self,
                                  textvariable=self.writeDataVar,
                                  width=3
                                  )
        self.writeData.grid(row=1,column=1)

        self.writeButton = tk.Button(self, text='Write', command=self.write)
        self.writeButton.grid(row=1,column=2)

        self.bind("<Return>",self.write_wa)
        self.bind("<Up>",self.inc_wa)
        self.bind("<Down>",self.dec_wa)
        self.bind("<Prior>",self.inc_256_wa)
        self.bind("<Next>",self.dec_256_wa)

    def genHex(self,*args):
        newtext = self.writeData.get()
        i=0
        while(i<len(newtext)):
            if not(newtext[i] <= 'f' and newtext[i] >= 'a' or newtext[i] <='F' and newtext[i] >= 'A' or newtext[i] <= '9' and newtext[i]>='0'):
                newtext = newtext[:i] + newtext[i+1:]
            else:
                i = i+1
        if len(newtext) > 2:
            newtext = newtext[0:2]
        if len(newtext) == 0:
            newtext = '0'
        self.writeDataVar.set(newtext)
        
    def connect(self):
        self.device = SwitchMatrix()
        if self.device.device is None:
            raise ValueError('Device connection failed.')

    def run(self):
        while True:
            if self.kill:
                break
            if self.device is None:
                continue
            time.sleep(1)
        print('end')

    def write(self):
        data = int(self.writeData.get(),16)
        if self.device is None:
            raise ValueError('Device is not connected.')
            return
        self.device.write(data)
        self.writeData.icursor(0)
        
    def write_wa(self,*args):
        self.write()

    def inc_wa(self,*args):
        data = int(self.writeData.get(),16) + 1
        if(data > 0xFFFF):
            data = data - 0x10000
        self.writeDataVar.set('%02X' % data)
    def inc_256_wa(self,*args): 
        data = int(self.writeData.get(),16) + 256
        if(data > 0xFFFF):
            data = data - 0x10000
        self.writeDataVar.set('%02X' % data)

    def dec_wa(self,*args):
        data = int(self.writeData.get(),16) - 1
        if(data < 0):
            data = 0x10000 + data
        self.writeDataVar.set('%02X' % data)
    
    def dec_256_wa(self,*args):
        data = int(self.writeData.get(),16) - 256
        if(data < 0):
            data = 0x10000 + data
        self.writeDataVar.set('%02X' % data)
app = Application()
app.title('SwitchMatrix')
app.connect()
app.mainloop()

