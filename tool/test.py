import tkinter as tk
class MyApp():
    def __init__(self):
        self.root = tk.Tk()

        # valid percent substitutions (from the Tk entry man page)
        # %d = Type of action (1=insert, 0=delete, -1 for others)
        # %i = index of char string to be inserted/deleted, or -1
        # %P = value of the entry if the edit is allowed
        # %s = value of entry prior to editing
        # %S = the text string being inserted or deleted, if any
        # %v = the type of validation that is currently set
        # %V = the type of validation that triggered the callback
        #      (key, focusin, focusout, forced)
        # %W = the tk name of the widget
        vcmd = self.root.register(self.OnValidate)
        w = tk.StringVar()
        w.set('asdfasf')
        self.entry = tk.Entry(self.root, validate="all", 
                              validatecommand=(vcmd, 
                '%d', '%i', '%P', '%s', '%S', '%v', '%V', '%W'), textvariable=w)
        self.entry.pack()
        self.but = tk.Button(self.root, text='quit', command=self.root.destroy)
        self.but.pack()
        self.root.mainloop()

    def OnValidate(self, d, i, P, s, S, v, V, W):
        print("OnValidate:")
        print("d='%s'" % d)
        print("i='%s'" % i)
        print("P='%s'" % P)
        print("s='%s'" % s)
        print("S='%s'" % S)
        print("v='%s'" % v)
        print("V='%s'" % V)
        print("W='%s'" % W)
        # only allow if the string is lowercase
        return True

app=MyApp()
