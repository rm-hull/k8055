#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Python version of libK8055 demo program
#
# Copyright (C) 2007 by Pjetur G. Hjaltason
#

import sys
from qt import *
#from qwt import *

import time, string
from pyk8055 import *
from frmk8055qt import *

class QTK8055(K8055QT):
	def __init__(self):
		K8055QT.__init__(self)

		# Not much sense in making a fancy menu yet
		#
		#puProg = QPopupMenu(self)
		#puProg.insertItem("Quit", Kapp.quit)

		#puHelp = QPopupMenu(self)
		#puHelp.insertItem("About", self.about)

		#self.menu = QMenuBar(self)
		#self.menu.insertItem("Program", puProg)
		#self.menu.insertItem("Help", puHelp)

		# enumerate devices
		BMask = SearchDevices()
		if not BMask:
			print "No K8055 devices found"
			exit

		# Enable those present
		self.CardButton1.setEnabled((BMask & 0x01) > 0)
		self.CardButton2.setEnabled((BMask & 0x02) > 0)
		self.CardButton3.setEnabled((BMask & 0x04) > 0)
		self.CardButton4.setEnabled((BMask & 0x08) > 0)

		# set the first one found as suggestion
		if (BMask & 0x01) > 0:
			self.card = 0
			self.CardButton1.setChecked(1)
		elif (BMask & 0x02) > 0:
			self.card = 1
			self.CardButton2.setChecked(1)
		elif (BMask & 0x04) > 0:
			self.card = 2
			self.CardButton3.setChecked(1)
		elif (BMask & 0x08) > 0:
			self.card = 3
			self.CardButton4.setChecked(1)

        	self.time = QTime.currentTime()     # get current time
        	internalTimer = QTimer( self )      # create internal timer
        	self.connect( internalTimer, SIGNAL("timeout()"), self.timeout )
        	internalTimer.start( 100 )         # emit signal every 1/10 second

		self.timeout()

	def AnalogTest(self):
		try:
			for step in range(0,255,10):
				self.BarAD1.setValue(step)
				self.BarAD2.setValue(step)
				time.sleep(0.1)
			# set analog output channel 1 to 200/255*5v = 3.9V
			for step in range(255,0,10):
				self.BarAD1.setValue(step)
				self.BarAD2.setValue(step)
				time.sleep(0.1)
				
			# read both analog inputs
			# note: this returns a list
			#res = self.k.ReadAllAnalog()
			#sres = res[1:]
			
			# There should be more interesting information in the messagebox ;-/
			
			QMessageBox.information( self,'Information', "AllOK","Ok")
		except:
			pass

	def DigitalTest(self):
		try:
			# Create a rotating display of digital outputs
			# while waiting for keypress on digital input 1
			# (longer than .2sec :) Loop at most 3 times 
			Input1High = False
			for i in range(1,3):
				d = 1
				self.k.WriteAllDigital(1)
				while Input1High == False and d < 128:
					Input1High = (self.k.ReadDigitalChannel(1) > 0)
					if Input1High: break
					time.sleep(0.2)	# wait .2 sec
					d *=2		# and continue rotating digital outputs
					#if d > 128: d=1;
					self.k.WriteAllDigital(d)
				if Input1High: break

			res = "Digital input 1=" + str(self.k.ReadDigitalChannel(1)) + "<br>"
			# read the counter on input 1
			res+= "Counter(1)=" + str(self.k.ReadCounter(1)) + "<br>"
			# read the counter on input 2
			res+= "Counter(2)=" + str(self.k.ReadCounter(2))
		
			# set even bits on digital outputs
			self.k.WriteAllDigital(170)
			time.sleep(1)
			# set odd bits on digital outputs
			self.k.WriteAllDigital(85)
			time.sleep(1)
			# Clear all digital outputs
			self.k.WriteAllDigital(0)

			# There should be more interesting information in the messagebox ;-/
			
			QMessageBox.information( self,'DigitalTest', res,"Ok")
		except:
			QMessageBox.warning( self,'DigitalTest', "Not OK","Ok")
			pass

	def EnableAll(self):
		self.OutputBox1.setEnabled(1)
		self.OutputBox2.setEnabled(1)
		self.OutputBox3.setEnabled(1)
		self.OutputBox4.setEnabled(1)
		self.OutputBox5.setEnabled(1)
		self.OutputBox6.setEnabled(1)
		self.OutputBox7.setEnabled(1)
		self.OutputBox8.setEnabled(1)
		self.SetAllDigitalButton.setEnabled(1)
		self.ClearAllDigitalButton.setEnabled(1)
		self.SetAllAnalogButton.setEnabled(1)
		self.ClearAllAnalogButton.setEnabled(1)
		self.comboCounter1.setEnabled(1)
		self.comboCounter2.setEnabled(1)
		self.ClearCounter1.setEnabled(1)
		self.ClearCounter2.setEnabled(1)
		self.TestDigitalButton.setEnabled(1)
		self.TestAnalogButton.setEnabled(1)
		self.BarAD1.setEnabled(1)
		self.BarAD2.setEnabled(1)
		self.BarDA1.setEnabled(1)
		self.BarDA2.setEnabled(1)

	def DisableAll(self):
		self.OutputBox1.setEnabled(0)
		self.OutputBox2.setEnabled(0)
		self.OutputBox3.setEnabled(0)
		self.OutputBox4.setEnabled(0)
		self.OutputBox5.setEnabled(0)
		self.OutputBox6.setEnabled(0)
		self.OutputBox7.setEnabled(0)
		self.OutputBox8.setEnabled(0)
		self.SetAllDigitalButton.setEnabled(0)
		self.ClearAllDigitalButton.setEnabled(0)
		self.SetAllAnalogButton.setEnabled(0)
		self.ClearAllAnalogButton.setEnabled(0)
		self.comboCounter1.setEnabled(0)
		self.comboCounter2.setEnabled(0)
		self.ClearCounter1.setEnabled(0)
		self.ClearCounter2.setEnabled(0)
		self.TestDigitalButton.setEnabled(0)
		self.TestAnalogButton.setEnabled(0)
		self.BarAD1.setEnabled(0)
		self.BarAD2.setEnabled(0)
		self.BarDA1.setEnabled(0)
		self.BarDA2.setEnabled(0)
#
## The QTimer::timeout() signal is received by this slot.
#

    	def timeout( self ):
        	#new_time = QTime.currentTime()                # get the current time
		if self.k != None:
			try:
				AllVal = self.k.ReadAllValues()
				#print AllVal,AllVal[2]
				self.BarDA1.setValue(255 - AllVal[2])
				self.BarDA2.setValue(255 - AllVal[3])
				self.DA1Value.setProperty("intValue",QVariant(AllVal[2]))
				self.DA2Value.setProperty("intValue",QVariant(AllVal[3]))
				self.Counter1Value.setProperty("intValue",QVariant(AllVal[4]))
				self.Counter2Value.setProperty("intValue",QVariant(AllVal[5]))

				if (AllVal[1] & 0x01) > 0:
					self.DigitalLed1.setState(KLed.On)
				else:
					self.DigitalLed1.setState(KLed.Off)
				if (AllVal[1] & 0x02) > 0:
					self.DigitalLed2.setState(KLed.On)
				else:
					self.DigitalLed2.setState(KLed.Off)
				if (AllVal[1] & 0x04) > 0:
					self.DigitalLed3.setState(KLed.On)
				else:
					self.DigitalLed3.setState(KLed.Off)
				if (AllVal[1] & 0x08) > 0:
					self.DigitalLed4.setState(KLed.On)
				else:
					self.DigitalLed4.setState(KLed.Off)
				if (AllVal[1] & 0x10) > 0:
					self.DigitalLed5.setState(KLed.On)
				else:
					self.DigitalLed5.setState(KLed.Off)
			except:
				print "K8055 Read failed"
				
	def about(self):
		QMessageBox.about( self, "About k8055qt",
        		"k8055qt is a small GUI program using libk8055\n\n"
        		"Copyright 2007 Pjetur G. Hjaltason.  "
        		"Distributed under GPL license\n\n"
        		"For technical support, study the source code or surf\n"
        		"http://sourceforge.net/projects/libk8055/\n" )

if __name__ == "__main__":
    	Kapp = QApplication(sys.argv)
    	QObject.connect(Kapp,SIGNAL("lastWindowClosed()"),Kapp,SLOT("quit()"))
    	w = QTK8055()
    	Kapp.setMainWidget(w)
    	w.show()
    	Kapp.exec_loop()


