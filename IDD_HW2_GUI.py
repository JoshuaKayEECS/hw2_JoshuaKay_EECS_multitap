#IDD HW2 Iteractive Device Design Text Entry GUI
#Josh Kay

import serial
import threading
import Queue
from Tkinter import *

#Serial Port Class
class SerialThread(threading.Thread):
	def __init__(self,queue):
		threading.Thread.__init__(self)
		self.queue = queue
		global closed
		closed = 0

	def run(self):
		s = serial.Serial('/dev/cu.usbmodem1411',9600)	
		while True:
			if closed == 1:	
				break
			elif s.inWaiting():
				text = s.readline(s.inWaiting())
				self.queue.put(text)


class Application(Frame):
        """ A GUI application for simple password entry. """

        def __init__(self,master):
                """ Initialize the Frame """
                Frame.__init__(self,master)
		self.i = 0
		self.index ="1.0"
		self.queue = Queue.Queue()
		thread = SerialThread(self.queue)
		thread.start()
                self.grid()
                self.create_widgets()

        def create_widgets(self):
                """Create text and call process serial"""
                self.instruction = Label(self, text = "IDD HW2: Text Entry",font=("chalkduster",24),fg="#000000fff",padx=10,pady=5)
                self.instruction.grid(row=0,column=0,columnspan=1,rowspan=1,sticky=W)

		self.message = Label(self, text = "Message:",font=("calibri",16))
		self.message.grid(row=1,column=0,sticky=W)

                self.text = Text(self,width=35,height=5,wrap=WORD,font=("calibri",14))
                self.text.grid(row=2,column=0,columnspan=2,sticky=W)

		self.send_button = Button(self)
		self.send_button["text"] = "Clear"
		self.send_button["command"] = self.send_message
		self.send_button.grid(row=5,column=0,sticky=W)	
			
		self.process_serial()
			
	
	def process_serial(self):
    		while self.queue.qsize():
        		try:   		
				self.num = int(self.queue.get())
				if self.num == -1:
					self.index = "1.%d" % self.i
					self.i += 1
					print "self.index %s" % self.index
				elif self.num == -2:
					if self.i != 0:
						self.i -= 1
					self.index = "1.%d" % self.i
					self.text.delete(self.index)
					print "Delete self.index %s " % self.index
				else:
					self.c = chr(self.num)
					self.text.delete(self.index)
					self.text.insert(self.index,self.c)
					print self.c
				#if self.queue.get() == '1':
				#	self.i += 1
				#	self.text.insert('end',self.i)
			except self.queue.empty:
				pass
		self.after(100, self.process_serial)

	def send_message(self):
		self.text.delete(1.0,'end')
		self.i = 0
		self.index = "1.0"	
	
def on_closing():
	#print "It's Over!"
	global closed
	closed = 1
	root.destroy()
	exit()

root = Tk()
root.title("IDD HW2 - Text Entry")
root.geometry("325x200")
root.protocol("WM_DELETE_WINDOW", on_closing)
#root.after(10, readSerial) # check serial again soon
app = Application(root)

root.mainloop()
