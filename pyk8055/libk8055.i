/* $Id: libk8055.i,v 1.3 2007/03/28 10:08:13 pjetur Exp $

   Copyright (C) 2007  by Pjetur G. Hjaltason


   Python wrapper for the libk8055 library using "swig"
   This file is part of the libk8055 Library.

   The libk8055 Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The libk8055 Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the
   Free Software Foundation, Inc.,
   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

   http://opensource.org/licenses/

*/
%module pyk8055
%include "typemaps.i"

/* for the ReadAllAnalog and ReadAllValues function */
%apply long *OUTPUT { long int *data1, long int *data2, long int *data3, long int *data4, long int *data5 };

%inline %{

/* For direct access to the DEBUG variable and global IO strings */
extern int DEBUG;
extern unsigned char data_in[9];
extern unsigned char data_out[9];
/*
    Extended functions for debugging the IO
*/

char strval[30];

/* returns 16byte hex string - 8 bytes */
char * RawInput()
        {
        int i;
        for(i=0; i< 8; i++) {
                sprintf((char *)&strval[i*2],"%02X",data_in[i]);
                }
        return (char *)strval;
        }

/* More print friendly format */
char * DumpInput()
        {
        int i;
        for(i=0; i< 8; i++) {
                sprintf((char *)&strval[i*3],"%02X:",data_in[i]);
                }
        strval[23] = '\0';
        return (char *)strval;
        }

char * DumpOutput()
        {
        int i;
        for(i=0; i< 8; i++) {
                sprintf((char *)&strval[i*3],"%02X:",data_out[i]);
                }
        strval[23] = '\0';
        return (char *)strval;
        }
%}

%{
extern int OpenDevice (long int BoardAddress);
extern int CloseDevice (void);
extern long int ReadAnalogChannel (long int Channelno);
extern int ReadAllAnalog (long int  *data1, long int *data2);
extern int OutputAnalogChannel (long int Channel, long int data);
extern int OutputAllAnalog (long int data1, long int data2);
extern int ClearAllAnalog (void);
extern int ClearAnalogChannel (long int Channel);
extern int SetAnalogChannel (long int Channel);
extern int SetAllAnalog (void);
extern int WriteAllDigital (long int data);
extern int ClearDigitalChannel (long int Channel);
extern int ClearAllDigital (void);
extern int SetDigitalChannel (long int Channel);
extern int SetAllDigital (void);
extern int ReadDigitalChannel (long int Channel);
extern long int ReadAllDigital (void);
extern int ResetCounter (long int counternr);
extern long int ReadCounter (long int CounterNo); 
extern int SetCounterDebounceTime (long int CounterNo, long int DebounceTime);
extern int ReadAllValues (long int  *data1, long int *data2, long int *data3, long int *data4, long int *data5);
extern int SetAllValues(int digitaldata, int addata1, int addata2);
extern int SetCounterDebounceTime(long CounterNo, long DebounceTime);
extern long SetCurrentDevice(long deviceno);
extern long SearchDevices(void);
extern char *Version(void);

%}
/* 

And here we create the class interface to the library

*/
%pythoncode %{
K8055_ERROR = -1
_K8055_CLOSED = -1

class k8055:
    "Class interface to the libk8055 library"
    def __init__(self,BoardAddress=None,debug=False):
        """Constructor, optional board open

         k=k8055()  # Does not connect to board
         k=k8055(1) # Init class and connect to board 1
         k=k8055(1,True) # connect to board 0 and enable debugging

        """
        self.Buttons = 0
        self.dev = _K8055_CLOSED
	self.Address = BoardAddress
        if debug == False:
            self.DebugOff()
        else:
            self.DebugOn()
        if BoardAddress != None:
            try:
                self.dev = self.OpenDevice(BoardAddress)
                # print "Open OK " + str(self.dev)
            except:
                self.dev = _K8055_CLOSED 
                raise IOError, "Could not open device"
                # print "Open error"

    def __str__(self):
        """String format (almost) as from K8055 program"""
        if self.__opentest() == True:    # Device open
            all = self.ReadAllValues()
            return str(all[1])+";"+str(all[2])+";"+str(all[3])+";"+str(all[4])+";"+str(all[5])
        else:
            return ""

    def __opentest(self):
        if self.dev == _K8055_CLOSED:
            return False
        else:
            return True

    def OpenDevice(self,BoardAddress):
        """Open the connection to K8055

        k=k8055()
        try;
           k.OpenDevice(0) # possible (address 0/1/2/3)
        except IOError:
            ...
        returns 0 if OK,
        Throws IOError if invalid board or not accessible 
        """
        if self.__opentest() == False:    # Not open yet
            self.dev = _pyk8055.OpenDevice(BoardAddress)
            if self.dev == K8055_ERROR:
                raise IOError, "Could not open device"
            # print "Open OK " + str(self.dev)
        return self.dev

    def CloseDevice(self):
        """Close the connection to K8055

        k.CloseDevice()

        retuns 0 if OK else -1
        """
        if self.dev != _K8055_CLOSED:
            ret = _pyk8055.CloseDevice()
            self.dev = _K8055_CLOSED
            return ret

    def OutputAnalogChannel(self,Channel,value=0):
        """Set analog output channel value, default 0 (0-255)"""
        if self.__opentest() == False : return K8055_ERROR
        return _pyk8055.OutputAnalogChannel(Channel,value)

    def ReadAnalogChannel(self,Channel):
        """Read data from analog input channel (1/2)"""
        if self.__opentest() == False : return K8055_ERROR
        return _pyk8055.ReadAnalogChannel(Channel)

    def ReadAllAnalog(self):
        """Read data from both analog input channels at once

        Returns list, [return-value,channel_data1, channel_data2]

        """
        if self.__opentest() == False : return K8055_ERROR
        return _pyk8055.ReadAllAnalog()

    def OutputAllAnalog(self,data1,data2):
        """Set both analog output channels at once (0-255,0-255)"""
        if self.__opentest() == False : return K8055_ERROR
        return _pyk8055.OutputAllAnalog(data1,data2)

    def ClearAllAnalog(self):
        """Set all (both) analog output channels to 0 (low)"""
        if self.__opentest() == False : return K8055_ERROR
        return _pyk8055.ClearAllAnalog()

    def ClearAnalogChannel(self,Channel):
        """Set analog output channel (1/2)to 0 (low)"""
        if self.__opentest() == False : return K8055_ERROR
        return _pyk8055.ClearAnalogChannel(Channel)

    def SetAnalogChannel(self,Channel):
        """Set analog output channel (1/2) to 0xFF (high)"""
        if self.__opentest() == False : return K8055_ERROR
        return _pyk8055.SetAnalogChannel(Channel)

    def SetAllAnalog(self):
        """Set all (both) analog output channels to 0xFF (high)"""
        if self.__opentest() == False : return K8055_ERROR
        return _pyk8055.SetAllAnalog()

    def WriteAllDigital(self,data):
        """Write digital output channel bitmask (0-255)"""
        if self.__opentest() == False : return K8055_ERROR
        return _pyk8055.WriteAllDigital(data)

    def ClearDigitalChannel(self,Channel):
        """Clear digital output channel (1-8)"""
        if self.__opentest() == False : return K8055_ERROR
        return _pyk8055.ClearDigitalChannel(Channel)

    def ClearAllDigital(self):
        """Set all digital output channels low (0)"""
        if self.__opentest() == False : return K8055_ERROR
        return _pyk8055.ClearAllDigital()

    def SetDigitalChannel(self,Channel):
        """Set digital output channel (1-8)"""
        if self.__opentest() == False : return K8055_ERROR
        return _pyk8055.SetDigitalChannel(Channel)

    def SetAllDigital(self):
        """Set all digital output channels high (1)"""
        if self.__opentest() == False : return K8055_ERROR
        return _pyk8055.SetAllDigital()

    def ReadDigitalChannel(self,Channel):
        """Read digital input channel (1-5), returns 0/1 (-1 on error)"""
        if self.__opentest() == False : return K8055_ERROR
        return _pyk8055.ReadDigitalChannel(Channel)

    def ReadAllDigital(self):
        """Read all digital input channels - bitmask

        returns mask 0x00 - 0x1f, 0x01 as channel 1, 0x02 as channel 2...
        retuns -1 on error

        """
        if self.__opentest() == False : return K8055_ERROR
        return _pyk8055.ReadAllDigital()

    def ResetCounter(self,CounterNo):
        """Reset input counter (1/2), input channel 1/2"""
        if self.__opentest() == False : return K8055_ERROR
        return _pyk8055.ResetCounter(CounterNo)

    def ReadCounter(self,CounterNo):
        """Read input counter (1/2), input channel 1/2"""
        if self.__opentest() == False : return K8055_ERROR
        return _pyk8055.ReadCounter(CounterNo)

    def SetCounterDebounceTime(self,CounterNo, DebounceTime):
        """Set counter debounce time on counter 1/2 (1-7450 ms)"""
        if self.__opentest() == False : return K8055_ERROR
        return _pyk8055.SetCounterDebounceTime(CounterNo,DebounceTime)

    # Makes no sense to switch to another class here
    def SetCurrentDevice(self):
        if self.__opentest() == False : return K8055_ERROR
        return _pyk8055.SetCurrentDevice(self.Address)

    # This function makes no sense in this context as
    # the device is already open
    #def SearchDevices(self):
        #return _pyk8055.SearchDevices()

    def DeviceAddress(self):
        return self.Address

    def IsOpen(self):
        return self.__opentest()

    def ReadAllValues(self):
        """Read data from all input channels at once

        Returns list, [return-value,digital input data, analog channel_data1, analog channel_data2, counter1, counter2]

        """
        if self.__opentest() == False : return K8055_ERROR
        return _pyk8055.ReadAllValues()
    def SetAllValues(self,digitaldata, addata1, addata2):
        if self.__opentest() == False : return K8055_ERROR
        return _pyk8055.SetAllValues(digitaldata, addata1, addata2)

    def Version(self):
        return _pyk8055.Version();

    def DebugOn(self):
        _pyk8055.cvar.DEBUG = 1
    def DebugOff(self):
        _pyk8055.cvar.DEBUG = 0

    def RawInput(self):
        return _pyk8055.RawInput()
    def DumpInput(self):
        return "In <-"+_pyk8055.DumpInput()
    def DumpOutput(self):
        return "Out->"+_pyk8055.DumpOutput()

%}
extern int OpenDevice (long int BoardAddress);
extern int CloseDevice (void);
extern long int ReadAnalogChannel (long int Channelno);
extern int ReadAllAnalog (long int *data1, long int *data2);
extern int OutputAnalogChannel (long int Channel, long int data);
extern int OutputAllAnalog (long int data1, long int data2);
extern int ClearAllAnalog (void);
extern int ClearAnalogChannel (long int Channel);
extern int SetAnalogChannel (long int Channel);
extern int SetAllAnalog (void);
extern int WriteAllDigital (long int data);
extern int ClearDigitalChannel (long int Channel);
extern int ClearAllDigital (void);
extern int SetDigitalChannel (long int Channel);
extern int SetAllDigital (void);
extern int ReadDigitalChannel (long int Channel);
extern long int ReadAllDigital (void);
extern int ResetCounter (long int counternr);
extern long int ReadCounter (long int CounterNo);
extern int SetCounterDebounceTime (long int CounterNo, long int DebounceTime);
extern int ReadAllValues (long int  *data1, long int *data2, long int *data3, long int *data4, long int *data5);
extern int SetAllValues(int digitaldata, int addata1, int addata2);
extern long SetCurrentDevice(long deviceno);
extern long SearchDevices(void);
extern char *Version(void);
