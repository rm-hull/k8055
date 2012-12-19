#!/usr/bin/env python
# $Id: pyplotA.py,v 1.2 2007/03/15 14:55:38 pjetur Exp $
#
# Simple plotting of analog input data from the K8055 board
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
        self.a1 = zeros(len(self.x), Float)
        self.a2 = zeros(len(self.x), Float)

        self.setTitle("Simple K8055 datascope")
        self.setAutoLegend(True)

        self.curve1 = self.insertCurve("Input 1")
        self.curve2 = self.insertCurve("Input 2")

        self.setCurvePen(self.curve1, QPen(Qt.red))
        self.setCurvePen(self.curve2, QPen(Qt.blue))

        # No automatic scaling, set y-scale 0-255
        self.setAxisScale(QwtPlot.yLeft,0,255,50)

        # set marker line in the middle - value 128
        mY = self.insertLineMarker("", QwtPlot.yLeft)      
        self.setMarkerYPos(mY, 128.0)

        self.setAxisTitle(QwtPlot.xBottom, "Time (seconds)")
        self.setAxisTitle(QwtPlot.yLeft, "Values")

        self.startTimer(50)

        self.k = k8055(0)
    # __init__()

    def timerEvent(self, e):

        # data moves from left to right:
        # shift data array right and assign new value data[0]

        self.a1 = concatenate((self.a1[:1], self.a1[:-1]), 1)
        self.a1[0] = self.k.ReadAnalogChannel(1)

        self.a2 = concatenate((self.a2[:1], self.a2[:-1]), 1)
        self.a2[0] = self.k.ReadAnalogChannel(2)

        self.setCurveData(self.curve1, self.x, self.a1)
        self.setCurveData(self.curve2, self.x, self.a2)

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
