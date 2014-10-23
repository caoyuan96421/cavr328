
import tkinter as tk
import time
from threading import Thread
from FPGADebugger import *

class Application(tk.Tk):
    def __init__(self, master=None):
        tk.Tk.__init__(self,master)
        self.grid()
        self.createWidgets()
        self.debugger = None
        self.thread = Thread(target = self.run, name = 'FPGA Debugger Daemon')
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

        tk.Label(self, text='Read: 0x').grid()
        
        self.readDataVar = tk.StringVar()
        self.readData = tk.Label(self, textvariable=self.readDataVar)
        self.readDataVar.set('0000')
        self.readData.grid(row=1,column=1)

        tk.Label(self, text='Write: 0x').grid()

        self.writeDataVar = tk.StringVar(value='0000')
        self.writeDataVar.trace("w", self.genHex)
        self.writeData = tk.Entry(self,
                                  textvariable=self.writeDataVar,
                                  width=5
                                  )
        self.writeData.grid(row=2,column=1)

        self.writeButton = tk.Button(self, text='Write', command=self.write)
        self.writeButton.grid(row=2,column=2)

        tk.Label(self,text='Clock: ').grid()
        self.clockSelect = tk.Spinbox(self, from_=0, to=15, state='readonly', width=3)
        self.clockSelect.grid(row=3,column=1)

        self.tickButton = tk.Button(self, text='Tick', command=self.tick)
        self.tickButton.grid(row=3,column=2)

        self.bind("<Return>",self.write_wa)
        self.bind("<Up>",self.inc_wa)
        self.bind("<Down>",self.dec_wa)
        self.bind("<Prior>",self.inc_256_wa)
        self.bind("<Next>",self.dec_256_wa)
        self.bind("<space>", self.tick_wa)

    def genHex(self,*args):
        newtext = self.writeData.get()
        i=0
        while(i<len(newtext)):
            if not(newtext[i] <= 'f' and newtext[i] >= 'a' or newtext[i] <='F' and newtext[i] >= 'A' or newtext[i] <= '9' and newtext[i]>='0'):
                newtext = newtext[:i] + newtext[i+1:]
            else:
                i = i+1
        if len(newtext) > 4:
            newtext = newtext[0:4]
        if len(newtext) == 0:
            newtext = '0'
        #print(newtext)
        self.writeDataVar.set(newtext)
        #print(self.writeDataVar.get())
    def connect(self):
        if self.debugger is None:
            self.debugger = FPGADebugger()
        if self.debugger.device is None:
            raise ValueError('Device connection failed.')

    def run(self):
        while True:
            if self.kill:
                break
            if self.debugger is None:
                continue
            data = self.debugger.read()
            self.readDataVar.set('%04X' % data)
            time.sleep(0.05)
        print('end')

    def write(self):
        data = int(self.writeData.get(),16)
        if self.debugger is None:
            raise ValueError('Device is not connected.')
            return
        self.debugger.write(data)
        self.writeData.icursor(0)

    def tick(self):
        clk = int(self.clockSelect.get())
        data = int(self.writeData.get(),16)
        if self.debugger is None:
            raise ValueError('Device is not connected.')
            return
        self.debugger.write(data & ~(1<<clk))
        self.debugger.write(data | (1<<clk))
        time.sleep(0.1)
        self.debugger.write(data & ~(1<<clk))
        time.sleep(0.1)
    def tick_wa(self,*args):
        self.tick()
        
    def write_wa(self,*args):
        self.write()

    def inc_wa(self,*args):
        data = int(self.writeData.get(),16) + 1
        if(data > 0xFFFF):
            data = data - 0x10000
        self.writeDataVar.set('%04X' % data)
    def inc_256_wa(self,*args): 
        data = int(self.writeData.get(),16) + 256
        if(data > 0xFFFF):
            data = data - 0x10000
        self.writeDataVar.set('%04X' % data)

    def dec_wa(self,*args):
        data = int(self.writeData.get(),16) - 1
        if(data < 0):
            data = 0x10000 + data
        self.writeDataVar.set('%04X' % data)
    
    def dec_256_wa(self,*args):
        data = int(self.writeData.get(),16) - 256
        if(data < 0):
            data = 0x10000 + data
        self.writeDataVar.set('%04X' % data)
app = Application()
app.title('Simple FPGA Debugger')
app.mainloop()

