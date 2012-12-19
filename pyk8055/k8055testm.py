#!/usr/bin/env python
# $Id: k8055testm.py,v 1.1 2007/03/28 10:10:32 pjetur Exp $
#
# Sample pyton test program for pyk8055 wrapper
# Scanning and testing multiple boards
#
from time import sleep
from pyk8055 import *

try:
	K8055_devices =[]
	BMask = SearchDevices()
	if not BMask:
		print "No K8055 devices found"
		exit
	
	if BMask & 0x01 : K8055_devices.append(k8055(0))
	if BMask & 0x02 : K8055_devices.append(k8055(1))
	if BMask & 0x04 : K8055_devices.append(k8055(2))
	if BMask & 0x08 : K8055_devices.append(k8055(3))

	for k in K8055_devices:
		k.SetCurrentDevice()
		# set analog output channel 1 to 200/255*5v = 3.9V
		k.OutputAnalogChannel(1,200)
		sleep(1)
		# set analog output channel low again = 0V
		k.OutputAnalogChannel(1,0)

		# read both analog inputs
		# note: this returns a list
		res = k.ReadAllAnalog()
		print "Analog status, board #",k.DeviceAddress(),res[1:]

		# Test class string function
		print "Status, board #",k.DeviceAddress(),str(k)

		# Set debounce time on counter 1
		k.SetCounterDebounceTime(1, 100)

		# reset counter 1
		k.ResetCounter(1)

	# Loop creating a rotating display of digital outputs
	# Loop 3 times
	for i in range(1,3):
		for k in K8055_devices:
			k.SetCurrentDevice()
			d = 1
			k.WriteAllDigital(1)
			while d <= 128:		# While this running
				if k.ReadDigitalChannel(1):
					print "Hello world (from board #%d)" %(k.DeviceAddress(),)
				sleep(0.2)	# wait .2 sec
				d *=2		# and continue rotating digital outputs
				k.WriteAllDigital(d)

	for k in K8055_devices:
		k.SetCurrentDevice()
		# read/print the counter on input 1
		print "Board # %d Counter #1=%d" % (k.DeviceAddress(),k.ReadCounter(1))
		# read/print the counter on input 2
		print "Board # %d Counter #2=%d" % (k.DeviceAddress(),k.ReadCounter(2))

	for k in K8055_devices:
		k.SetCurrentDevice()
		# set even bits on digital outputs
		k.WriteAllDigital(170)

	sleep(1)

	for k in K8055_devices:
		k.SetCurrentDevice()
		# set odd bits on digital outputs
		k.WriteAllDigital(85)

	sleep(1)
	
	for k in K8055_devices:
		k.SetCurrentDevice()
		# Clear all digital outputs
		k.WriteAllDigital(0)

	# and close
	for k in K8055_devices:
		k.SetCurrentDevice()
		k.CloseDevice()

except KeyboardInterrupt:
	for k in K8055_devices:
		k.SetCurrentDevice()
		k.WriteAllDigital(0)
		k.CloseDevice()
	exit

except IOError:
	print "Could not open Device"

