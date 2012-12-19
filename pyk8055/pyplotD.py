#!/usr/bin/env python
# $Id: pyplotD.py,v 1.2 2007/03/15 14:55:38 pjetur Exp $
#
# Simple plotting of digital input data from the K8055 board
#
# based on the running plot sample from pyQwt
# The Python version of qwt-*/examples/data_plot/data_plot.cpp

import random, sys
from qt import *
from qwt import *
from Numeric import *
from pyk8055 import *

class DataPlot(QwtPlot):

    def __init__(self, *args):
        QwtPlot.__init__(self, *args)

        # Initialize data
        self.x = arrayrange(0.0, 100.1, 0.5)
        self.d1 = 0.0 + zeros(len(self.x), Float)
        self.d2 = 1.0 + zeros(len(self.x), Float)	# shift data up 1
        self.d3 = 2.0 + zeros(len(self.x), Float)	# Shift data up 2...
        self.d4 = 3.0 + zeros(len(self.x), Float)
        self.d5 = 4.0 + zeros(len(self.x), Float)

        self.setTitle("Simple K8055 datascope")
        self.setAutoLegend(True)

        self.curve1 = self.insertCurve("Input 1")
        self.curve2 = self.insertCurve("Input 2")
        self.curve3 = self.insertCurve("Input 3")
        self.curve4 = self.insertCurve("Input 4")
        self.curve5 = self.insertCurve("Input 5")

        self.setCurvePen(self.curve1, QPen(Qt.red))
        self.setCurvePen(self.curve2, QPen(Qt.blue))
        self.setCurvePen(self.curve3, QPen(Qt.green))
        self.setCurvePen(self.curve4, QPen(Qt.black))
        self.setCurvePen(self.curve5, QPen(Qt.cyan))

        # Make data plot shape square
        self.setCurveStyle(self.curve1, QwtCurve.Steps)
        self.setCurveStyle(self.curve2, QwtCurve.Steps)
        self.setCurveStyle(self.curve3, QwtCurve.Steps)
        self.setCurveStyle(self.curve4, QwtCurve.Steps)
        self.setCurveStyle(self.curve5, QwtCurve.Steps)

        # Fixed axis here from 0 to 5
        self.setAxisScale(QwtPlot.yLeft,0,5,1)

        self.setAxisTitle(QwtPlot.xBottom, "Time (seconds)")
        self.setAxisTitle(QwtPlot.yLeft, "Values")

        self.startTimer(50)

        # init the K8055 board
        self.k = k8055(0)
    # __init__()

    def timerEvent(self, e):

        # data moves from left to right:
        # shift data array right and assign new value data[0]

        self.d1 = concatenate((self.d1[:1], self.d1[:-1]), 1)
        self.d1[0] = self.k.ReadDigitalChannel(1) * 0.95

        self.d2 = concatenate((self.d2[:1], self.d2[:-1]), 1)
        self.d2[0] = self.k.ReadDigitalChannel(2) * 0.95 + 1	# Shift data up 1

        self.d3 = concatenate((self.d3[:1], self.d3[:-1]), 1)
        self.d3[0] = self.k.ReadDigitalChannel(3) * 0.95 + 2	# Shift data up 2...

        self.d4 = concatenate((self.d4[:1], self.d4[:-1]), 1)
        self.d4[0] = self.k.ReadDigitalChannel(4) * 0.95 + 3

        self.d5 = concatenate((self.d5[:1], self.d5[:-1]), 1)
        self.d5[0] = self.k.ReadDigitalChannel(5) * 0.95 + 4

        self.setCurveData(self.curve1, self.x, self.d1)
        self.setCurveData(self.curve2, self.x, self.d2)
        self.setCurveData(self.curve3, self.x, self.d3)
        self.setCurveData(self.curve4, self.x, self.d4)
        self.setCurveData(self.curve5, self.x, self.d5)

        self.replot()

    # timerEvent()

# class DataPlot

def main(args): 
    app = QApplication(args)
    demo = make()
    app.setMainWidget(demo)
    app.exec_loop()

# main()

def make():
    demo = DataPlot()
    demo.resize(500, 300)
    demo.show()
    return demo

# make()

# Admire
if __name__ == '__main__':
    main(sys.argv)

