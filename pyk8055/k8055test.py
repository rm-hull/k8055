#!/usr/bin/env python
# $Id: k8055test.py,v 1.3 2007/03/28 10:09:34 pjetur Exp $
#
# Sample pyton test program for pyk8055 wrapper
# Testing single board
#
from time import sleep
from pyk8055 import *

try:
	# CHANGE PORT HERE IF NEEDED
	# Open device port 0
	k = k8055(0)

	# set analog output channel 1 to 200/255*5v = 3.9V
	k.OutputAnalogChannel(1,200)
	sleep(1)
	# set analog output channel low again = 0V
	k.OutputAnalogChannel(1,0)

	# read both analog inputs
	# note: this returns a list
	res = k.ReadAllAnalog()
	print res[1:]

	# Test class string function
	print str(k)

	# Set debounce time on counter 1
	k.SetCounterDebounceTime(1, 100)

	# reset counter 1
	k.ResetCounter(1)

	# create a rotating display of digital outputs
	# while waiting for keypress on digital input 1
	# (longer than .2sec :) 
	Loop,dInput1,d = 0,0,1
	k.WriteAllDigital(1)
	while Loop < 3 or dInput1 == 0:		# is key 1 down (input 1 high)
		dInput1 = k.ReadDigitalChannel(1)
		Loop += 1
		sleep(0.2)	# wait .2 sec
		d *=2		# and continue rotating digital outputs
		if d > 128: d=1;
		k.WriteAllDigital(d)

	print "Digital input 1=",dInput1

	# read the counter on input 1
	print "Counter(1)=",k.ReadCounter(1)
	# read the counter on input 2
	print "Counter(2)=",k.ReadCounter(2)

	# set even bits on digital outputs
	k.WriteAllDigital(170)
	sleep(1)
	# set odd bits on digital outputs
	k.WriteAllDigital(85)
	sleep(1)
	# Clear all digital outputs
	k.WriteAllDigital(0)

	# and close
	k.CloseDevice()

except IOError:
	print "Could not open Device"


