/* $Id: libk8055.c,v 1.7 2008/08/20 17:00:55 mr_brain Exp $

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

   Copyleft (C) 2005 by Sven Lindberg
     k8055@k8055.mine.nu

   Copyright (C) 2007 by Pjetur G. Hjaltason
       pjetur@pjetur.net
       Commenting, general rearrangement of code, bugfixes,
       python interface with swig and simple k8055 python class


	Input packet format

	+---+---+---+---+---+---+---+---+
	|DIn|Sta|A1 |A2 |   C1  |   C2  |
	+---+---+---+---+---+---+---+---+
	DIn = Digital input in high nibble, except for input 3 in 0x01
	Sta = Status, Board number + 1
	A1  = Analog input 1, 0-255
	A2  = Analog input 2, 0-255
	C1  = Counter 1, 16 bits (lsb)
	C2  = Counter 2, 16 bits (lsb)

	Output packet format

	+---+---+---+---+---+---+---+---+
	|CMD|DIG|An1|An2|Rs1|Rs2|Dbv|Dbv|
	+---+---+---+---+---+---+---+---+
	CMD = Command 
	DIG = Digital output bitmask
	An1 = Analog output 1 value, 0-255
	An2 = Analog output 2 value, 0-255
	Rs1 = Reset counter 1, command 3
	Rs2 = Reset counter 3, command 4
	Dbv = Debounce value for counter 1 and 2, command 1 and 2

	Or split by commands

	Cmd 0, Reset ??
	Cmd 1, Set debounce Counter 1
	+---+---+---+---+---+---+---+---+
	|CMD|   |   |   |   |   |Dbv|   |
	+---+---+---+---+---+---+---+---+
	Cmd 2, Set debounce Counter 2
	+---+---+---+---+---+---+---+---+
	|CMD|   |   |   |   |   |   |Dbv|
	+---+---+---+---+---+---+---+---+
	Cmd 3, Reset counter 1
	+---+---+---+---+---+---+---+---+
	| 3 |   |   |   | 00|   |   |   |
	+---+---+---+---+---+---+---+---+
	Cmd 4, Reset counter 2
	+---+---+---+---+---+---+---+---+
	| 4 |   |   |   |   | 00|   |   |
	+---+---+---+---+---+---+---+---+
	cmd 5, Set analog/digital
	+---+---+---+---+---+---+---+---+
	| 5 |DIG|An1|An2|   |   |   |   |
	+---+---+---+---+---+---+---+---+

**/

#include <string.h>
#include <stdio.h>
#include <usb.h>
#include <assert.h>
#include <math.h>
#include "k8055.h"

#define STR_BUFF 256
#define PACKET_LEN 8

#define K8055_IPID 0x5500
#define VELLEMAN_VENDOR_ID 0x10cf
#define K8055_MAX_DEV 4

#define USB_OUT_EP 0x01	/* USB output endpoint */
#define USB_INP_EP 0x81 /* USB Input endpoint */

#define USB_TIMEOUT 20
#define K8055_ERROR -1

#define DIGITAL_INP_OFFSET 0
#define DIGITAL_OUT_OFFSET 1
#define ANALOG_1_OFFSET 2
#define ANALOG_2_OFFSET 3
#define COUNTER_1_OFFSET 4
#define COUNTER_2_OFFSET 6

#define CMD_RESET 0x00
#define CMD_SET_DEBOUNCE_1 0x01
#define CMD_SET_DEBOUNCE_2 0x01
#define CMD_RESET_COUNTER_1 0x03
#define CMD_RESET_COUNTER_2 0x04
#define CMD_SET_ANALOG_DIGITAL 0x05

/* set debug to 0 to not print excess info */
int DEBUG = 0;

/* variables for usb */
static struct usb_bus *bus, *busses;
static struct usb_device *dev;

/* globals for datatransfer */
struct k8055_dev {
    unsigned char data_in[PACKET_LEN+1];
    unsigned char data_out[PACKET_LEN+1];
    struct usb_dev_handle *device_handle;
    int DevNo;
};

static struct k8055_dev k8055d[K8055_MAX_DEV];
static struct k8055_dev *CurrDev;

/* Keep these globals for now */
unsigned char *data_in, *data_out;

/* char* device_id[]; */

/* Initialize the usb library - only once */
static void init_usb(void)
{
    static int Done = 0;	/* Only need to do this once */
    if (!Done) 
    {
        usb_init();
        usb_find_busses();
        usb_find_devices();
        busses = usb_get_busses();
        Done = 1;
    }
}
/* Actual read of data from the device endpoint, retry 3 times if not responding ok */
static int ReadK8055Data(void)
{
    int read_status = 0, i = 0;

    if (CurrDev->DevNo == 0) return K8055_ERROR;

    for(i=0; i < 3; i++)
        {
        read_status = usb_interrupt_read(CurrDev->device_handle, USB_INP_EP, (char *)CurrDev->data_in, PACKET_LEN, USB_TIMEOUT);
        if ((read_status == PACKET_LEN) && (CurrDev->data_in[1] == CurrDev->DevNo )) return 0;
        if ((read_status == PACKET_LEN) && (CurrDev->data_in[1] == CurrDev->DevNo +10)) return 0; /* works with K8055N / VM110N */
        if (DEBUG)
            fprintf(stderr, "Read retry\n");
        }
    return K8055_ERROR;
}

/* Actual write of data to the device endpont, retry 3 times if not reponding correctly */
static int WriteK8055Data(unsigned char cmd)
{
    int write_status = 0, i = 0;

    if (CurrDev->DevNo == 0) return K8055_ERROR;

    CurrDev->data_out[0] = cmd;
    for(i=0; i < 3; i++)
        {
        write_status = usb_interrupt_write(CurrDev->device_handle, USB_OUT_EP, (char *)CurrDev->data_out, PACKET_LEN, USB_TIMEOUT);
        if (write_status == PACKET_LEN) return 0;
        if (DEBUG)
            fprintf(stderr, "Write retry\n");
        }
    return K8055_ERROR;
}

/* If device is owned by some kernel driver, try to disconnect it and claim the device*/
static int takeover_device(usb_dev_handle * udev, int interface)
{
    char driver_name[STR_BUFF];

    memset(driver_name, 0, STR_BUFF);
    int ret = K8055_ERROR;

    assert(udev != NULL);
    ret = usb_get_driver_np(udev, interface, driver_name, sizeof(driver_name));
    if (ret == 0)
    {
        if (DEBUG)
            fprintf(stderr, "Got driver name: %s\n", driver_name);
        if (0 > usb_detach_kernel_driver_np(udev, interface))
        {
            if (DEBUG)
                fprintf(stderr, "Disconnect OS driver: %s\n", usb_strerror());
        }
        else if (DEBUG)
            fprintf(stderr, "Disconnected OS driver: %s\n", usb_strerror());
    }
    else if (DEBUG)
        fprintf(stderr, "Get driver name: - %s\n", usb_strerror());

    /* claim interface */
    if (usb_claim_interface(udev, interface) < 0)
    {
        if (DEBUG)
            fprintf(stderr, "Claim interface error: %s\n", usb_strerror());
        return K8055_ERROR;
    }
    else
        usb_set_altinterface(udev, interface);
    usb_set_configuration(udev, 1);

    if (DEBUG)
        {
        fprintf(stderr, "Found interface %d\n", interface);
        fprintf(stderr, "Took over the device\n");
        }

    return 0;
}

/* Open device - scan through usb busses looking for the right device,
   claim it and then open the device
*/
int OpenDevice(long BoardAddress)
{

    int ipid;

    /* init USB and find all of the devices on all busses */
    init_usb();

    /* ID of the welleman board is 5500h + address config */
    if (BoardAddress >= 0 && BoardAddress < K8055_MAX_DEV)
        ipid = K8055_IPID + (int)BoardAddress;
    else
        return K8055_ERROR;              /* throw error instead of being nice */

    /* start looping through the devices to find the correct one */
    for (bus = busses; bus; bus = bus->next)
    {
        for (dev = bus->devices; dev; dev = dev->next)
        {
            if ((dev->descriptor.idVendor == VELLEMAN_VENDOR_ID) &&
                (dev->descriptor.idProduct == ipid))
            {
                CurrDev = &k8055d[BoardAddress];
                CurrDev->device_handle = usb_open(dev);
                if (DEBUG)
                    fprintf(stderr,
                            "Velleman Device Found @ Address %s Vendor 0x0%x Product ID 0x0%x\n",
                            dev->filename, dev->descriptor.idVendor,
                            dev->descriptor.idProduct);
                if (takeover_device(CurrDev->device_handle, 0) < 0)
                {
                    if (DEBUG)
                        fprintf(stderr,
                                "Can not take over the device from the OS driver\n");
                    usb_close(CurrDev->device_handle);   /* close usb if we fail */
                    return K8055_ERROR;  /* throw K8055_ERROR to show that OpenDevice failed */
                }
                else
                {
                    CurrDev->DevNo = BoardAddress + 1; /* Mark as open and valid */
                    SetCurrentDevice(BoardAddress);
                    memset(CurrDev->data_out,0,PACKET_LEN);	/* Write cmd 0, read data */
                    WriteK8055Data(CMD_RESET);
                    if (ReadK8055Data() == 0)
                       return BoardAddress;		/* This function should return board address */
                    else
                       return K8055_ERROR;
                }
            }
        }
    }
    if (DEBUG)
        fprintf(stderr, "Could not find Velleman k8055 with address %d\n",
                (int)BoardAddress);
    return K8055_ERROR;
}

/* Close the Current device */
int CloseDevice()
{
     int rc;

     if (CurrDev->DevNo == 0)
     {
         if (DEBUG)
             fprintf(stderr, "Current device is not open\n" );
         return 0;
     }
     rc = usb_close(CurrDev->device_handle);
     if (rc >= 0)
     {
         CurrDev->DevNo = 0;  /* Not active nay more */
         CurrDev->device_handle = NULL;
     }
     return rc;
}

/* New function in version 2 of Velleman DLL, should return deviceno if OK */
long SetCurrentDevice(long deviceno)
{
    if (deviceno >= 0 && deviceno < K8055_MAX_DEV)
    {
        if (k8055d[deviceno].DevNo != 0)
        {
            CurrDev  = &k8055d[deviceno];
            data_in  = CurrDev->data_in;
            data_out = CurrDev->data_out;
            return deviceno;
        }
    }
    return K8055_ERROR;

}

/* New function in version 2 of Velleman DLL, should return devices-found bitmask or 0*/
long SearchDevices(void)
{
    int retval = 0;
    init_usb();
    /* start looping through the devices to find the correct one */
    for (bus = busses; bus; bus = bus->next)
    {
        for (dev = bus->devices; dev; dev = dev->next)
        {
            if (dev->descriptor.idVendor == VELLEMAN_VENDOR_ID) { 
               if(dev->descriptor.idProduct == K8055_IPID + 0) retval |= 0x01;
               if(dev->descriptor.idProduct == K8055_IPID + 1) retval |= 0x02;
               if(dev->descriptor.idProduct == K8055_IPID + 2) retval |= 0x04;
               if(dev->descriptor.idProduct == K8055_IPID + 3) retval |= 0x08;
            /* else some other kind of Velleman board */
            }
        }
    }
    return retval; 
}

long ReadAnalogChannel(long Channel)
{
    if (Channel == 1 || Channel == 2)
    {
        if ( ReadK8055Data() == 0)
        {
            if (Channel == 2)
                return CurrDev->data_in[ANALOG_2_OFFSET];
            else
                return CurrDev->data_in[ANALOG_1_OFFSET];
        }
        else
            return K8055_ERROR;
    }
    else
        return K8055_ERROR;
}

int ReadAllAnalog(long *data1, long *data2)
{
    if (ReadK8055Data() == 0)
    {
        *data1 = CurrDev->data_in[ANALOG_1_OFFSET];
        *data2 = CurrDev->data_in[ANALOG_2_OFFSET];
        return 0;
    }
    else
        return K8055_ERROR;
}

int OutputAnalogChannel(long Channel, long data)
{
    if (Channel == 1 || Channel == 2)
    {
        if (Channel == 2)
            CurrDev->data_out[ANALOG_2_OFFSET] = (unsigned char)data;
        else
            CurrDev->data_out[ANALOG_1_OFFSET] = (unsigned char)data;

        return WriteK8055Data(CMD_SET_ANALOG_DIGITAL);
    }
    else
        return K8055_ERROR;
}

int OutputAllAnalog(long data1, long data2)
{
    CurrDev->data_out[2] = (unsigned char)data1;
    CurrDev->data_out[3] = (unsigned char)data2;

    return WriteK8055Data(CMD_SET_ANALOG_DIGITAL);
}

int ClearAllAnalog()
{
    return OutputAllAnalog(0, 0);
}

int ClearAnalogChannel(long Channel)
{
    if (Channel == 1 || Channel == 2)
    {
        if (Channel == 2)
            return OutputAnalogChannel(2, 0);
        else
            return OutputAnalogChannel(1, 0);
    }
    else
        return K8055_ERROR;
}

int SetAnalogChannel(long Channel)
{
    if (Channel == 1 || Channel == 2)
    {
        if (Channel == 2)
            return OutputAnalogChannel(2, 0xff);
        else
            return OutputAnalogChannel(1, 0xff);
    }
    else
        return K8055_ERROR;

}

int SetAllAnalog()
{
    return OutputAllAnalog(0xff, 0xff);
}

int WriteAllDigital(long data)
{
    CurrDev->data_out[1] = (unsigned char)data;
    return WriteK8055Data(CMD_SET_ANALOG_DIGITAL);
}

int ClearDigitalChannel(long Channel)
{
    unsigned char data;

    if (Channel > 0 && Channel < 9)
    {
	data = CurrDev->data_out[1] & ~(1 << (Channel-1));
        return WriteAllDigital(data);
    }
    else
        return K8055_ERROR;
}

int ClearAllDigital()
{
    return WriteAllDigital(0x00);
}

int SetDigitalChannel(long Channel)
{
    unsigned char data;

    if (Channel > 0 && Channel < 9)
    {
        data = CurrDev->data_out[1] | (1 << (Channel-1));
        return WriteAllDigital(data);
    }
    else
        return K8055_ERROR;
}

int SetAllDigital()
{
    return WriteAllDigital(0xff);
}

int ReadDigitalChannel(long Channel)
{
    int rval;
    if (Channel > 0 && Channel < 9)
    {
        if ((rval = ReadAllDigital()) == K8055_ERROR) return K8055_ERROR;
        return ((rval & (1 << (Channel-1))) > 0);
    }
    else
        return K8055_ERROR;
}

long ReadAllDigital()
{
    int return_data = 0;

    if (ReadK8055Data() == 0)
    {
        return_data = (
                ((CurrDev->data_in[0] >> 4) & 0x03) |  /* Input 1 and 2 */
                ((CurrDev->data_in[0] << 2) & 0x04) |  /* Input 3 */
                ((CurrDev->data_in[0] >> 3) & 0x18) ); /* Input 4 and 5 */
        return return_data;
    }
    else
        return K8055_ERROR;
}

int ReadAllValues(long int *data1, long int * data2, long int * data3, long int * data4, long int * data5)
{
    if (ReadK8055Data() == 0)
    {
        *data1 = (
                ((CurrDev->data_in[0] >> 4) & 0x03) |  /* Input 1 and 2 */
                ((CurrDev->data_in[0] << 2) & 0x04) |  /* Input 3 */
                ((CurrDev->data_in[0] >> 3) & 0x18) ); /* Input 4 and 5 */
        *data2 = CurrDev->data_in[ANALOG_1_OFFSET];
        *data3 = CurrDev->data_in[ANALOG_2_OFFSET];
        *data4 = *((short int *)(&CurrDev->data_in[COUNTER_1_OFFSET]));
        *data5 = *((short int *)(&CurrDev->data_in[COUNTER_2_OFFSET]));
        return 0;
    }
    else
        return K8055_ERROR;
}

int SetAllValues(int DigitalData, int AdData1, int AdData2)
{
    CurrDev->data_out[1] = (unsigned char)DigitalData;
    CurrDev->data_out[2] = (unsigned char)AdData1;
    CurrDev->data_out[3] = (unsigned char)AdData2;

    return WriteK8055Data(CMD_SET_ANALOG_DIGITAL);
}

int ResetCounter(long CounterNo)
{
    if (CounterNo == 1 || CounterNo == 2)
    {
        CurrDev->data_out[0] = 0x02 + (unsigned char)CounterNo;  /* counter selection */
        CurrDev->data_out[3 + CounterNo] = 0x00;
        return WriteK8055Data(CurrDev->data_out[0]);
    }
    else
        return K8055_ERROR;
}

long ReadCounter(long CounterNo)
{
    if (CounterNo == 1 || CounterNo == 2)
    {
        if (ReadK8055Data() == 0)
        {
            if (CounterNo == 2)
                return *((short int *)(&CurrDev->data_in[COUNTER_2_OFFSET]));
            else
                return *((short int *)(&CurrDev->data_in[COUNTER_1_OFFSET]));
        }
        else
            return K8055_ERROR;
    }
    else
        return K8055_ERROR;
}

int SetCounterDebounceTime(long CounterNo, long DebounceTime)
{
    float value;

    if (CounterNo == 1 || CounterNo == 2)
    {
        CurrDev->data_out[0] = (unsigned char)CounterNo;
        /* the velleman k8055 use a exponetial formula to split up the
           DebounceTime 0-7450 over value 1-255. I've tested every value and
           found that the formula dbt=0,338*value^1,8017 is closest to
           vellemans dll. By testing and measuring times on the other hand I
           found the formula dbt=0,115*x^2 quite near the actual values, a
           little below at really low values and a little above at really
           high values. But the time set with this formula is within +-4% */
        if (DebounceTime > 7450)
            DebounceTime = 7450;
        value = sqrtf(DebounceTime / 0.115);
        if (value > ((int)value + 0.49999999))  /* simple round() function) */
            value += 1;
        CurrDev->data_out[5 + CounterNo] = (unsigned char)value;
        if (DEBUG)
            fprintf(stderr, "Debouncetime%d value for k8055:%d\n",
                    (int)CounterNo, CurrDev->data_out[5 + CounterNo]);
        return WriteK8055Data(CurrDev->data_out[0]);
    }
    else
        return K8055_ERROR;
}

char * Version(void)
{
    return(VERSION);
}
