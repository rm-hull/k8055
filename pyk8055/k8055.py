#!/usr/bin/env python
# $Id: k8055.py,v 1.2 2007/03/15 14:50:55 pjetur Exp $
"""
Python version of K8055 command line program

Copyright (C) 2007 by Pjetur G. Hjaltason

Syntax : python k8055.py [-p:(number)] [-d:(value)] [-a1:(value)] [-a2:(value)]
             [-num:(number) [-delay:(number)] [-dbt1:(value)]
             [-dbt2:(value)] [-reset1] [-reset2] [-debug] [-h|--help]
	-p:(number)	Set board number (0/1/2/3)
	-d:(value)	Set digital output value (bitmask, 8 bits in decimal)
	-a1:(value)	Set analog output 1 value (0-255)
	-a2:(value)	Set analog output 2 value (0-255)
	-num:(number)   Set number of measures (-1 for no read)
	-delay:(number) Set delay between two measures (in msec)
	-dbt1:(value)   Set debounce time for counter 1 (in msec)
	-dbt2:(value)   Set debounce time for counter 2 (in msec)
	-reset1		Reset counter 1
	-reset2		Reset counter 2
	-debug		Activate debug mode
	-h or --help	Print this text

Example : python k8055.py -p:1 -d:147 -a1:25 -a2:203

NOTE:
	Because of the nature of commands sent to the K8055 board, this
	program has no way of knowing previous state of the analog or
	digial outputs, thus each run of this command will clear the previous
	state - and set a new state - of the analog and digital outputs

	See header of libk8055.c for more details of K8055 commands
"""
import sys
from time import sleep, time
from pyk8055 import *

def main(argv):
	# preset values
	p , db = 0,0		# Port, debug
	r1, r2 = 0,0		# reset1, reset2
	dl = -1			# delay
	db1,db2 = -1,-1		# debounce1, debounce2
	a1,a2,d = -1,-1,-1	# analog1, analog2, digital
	nm = 1			# number of times to read, default once

	# not the standard getopts way, but...
	for arg in argv:
		if arg in ("-h", "--help"):
			print __doc__
			sys.exit(0)
		elif arg[:4] == "-a1:"     : a1  = int(arg[4:])
		elif arg[:4] == "-a2:"     : a2  = int(arg[4:])
		elif arg[:5] == "-num:"    : nm  = int(arg[5:])
		elif arg[:3] == "-d:"      : d   = int(arg[3:])
		elif arg[:3] == "-p:"      : p   = int(arg[3:])
		elif arg[:7] == "-delay:"  : dl  = int(arg[7:])
		elif arg[:6] == "-dbt1:"   : db1 = int(arg[6:])
		elif arg[:6] == "-dbt2:"   : db2 = int(arg[6:])
		elif arg     == "-reset1"  : r1  = 1
		elif arg     == "-reset2"  : r2  = 1
		elif arg     == "-debug"   : db  = 1
		else:
			print __doc__
			sys.exit(0)

	try:
		# Open device
		if db == 1:	k = k8055(p,True)	# Debug mode
		else:		k = k8055(p)

		# set requested
		if r1  !=  0:	k.ResetCounter(1)
		if r2  !=  0:	k.ResetCounter(2)
		if db1 != -1:	k.SetCounterDebounceTime(1, db1)
		if db2 != -1:	k.SetCounterDebounceTime(2, db2)

		# Try to set all digital/analog values at the same time
		if (d != -1 and a1 != -1 and a2 != -1) : SetAllValues(d,a1,a2)
		elif (d  != -1 and a1 != -1 ) : SetAllValues(d,a1,0)
		elif (d  != -1 and a2 != -1 ) : SetAllValues(d,0,a2)
		elif (a1 != -1 and a2 != -1 ) : SetAllValues(0,a1,a2)
		# Else only one specified 
		else:
			if d   != -1:	k.WriteAllDigital(d)
			if a1  != -1:	k.OutputAnalogChannel(1,a1)
			if a2  != -1:	k.OutputAnalogChannel(2,a2)

		# Now we loop (or not) for the specified number of times
		# reading all the input values

		# Each read of all values will take about 2-8ms, at least
		# on my setup - so no sense in using delay less than 10
		if (dl > 0) & (dl < 10): dl = 10

		if (nm > 0) & (dl > 0):
			tst = tstart = time()
			i = 0
			while nm > 0:
				ds = str(k)
				tnow = time()
				tms = (tnow - tstart) * 1000.0
				delta= dl - (tms - dl*i)
				#tst = tnow
				print str(int(tms+0.4999))+";"+ds
				sleep((delta)/1000.0)	# compensate for read time
				nm -= 1
				i += 1
		elif nm > 0:
			print "0;"+ str(k)

		k.CloseDevice()

	except IOError:
		print "Could not open the k8055 (port:%s)" % str(p)

main(sys.argv[1:])
