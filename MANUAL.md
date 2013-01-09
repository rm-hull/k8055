k8055
=====

* * * * *

NAME
----
k8055 − read from/write to the velleman k8055 USB I/O board

SYNOPSIS
--------

    k8055 [-p: board_address] [option1] [option2] ...

DESCRIPTION
-----------
**k8055** is a commandline util for the velleman k8055 USB I/O board.
Use this tool to read all inputs and counters or to set digital outputs,
analog outputs, reset timers or set debounce time for the two timers.
The command can be run by root or any other user with access to use USB.
The k8055 require libusb-0.1.9 or newer and kernel 2.4.18 or never.

OPTIONS
-------
**−p:** *number*

the address of the k8055 board. A number between 0 and 3.

**−d:** *value*

set digital output value 0-255 (8bits in decimal)

**−a1:** *value*

set the 8-bit analog output 1. Valid value 0-255 (0=min, 255=max)

**−a2:** *value*

set the 8-bit analog output 2. Valid value 0-255 (0=min, 255=max)

**−num:** *number*

set number of measurements to return.

**−delay:** *number* 

set delay between two measures in milliseconds. Use together with −num
option.

**−dpt1:** *value*

set the value of the debounce time for timer1 in milliseconds (0-5000).
Debounce time is the time that the counter input need to be active
before it counts the value.

**−dpt2:** *value*

set the value of the debounce time for timer2 in milliseconds (0-5000).

**−reset1** 

reset counter 1 value to 0.

**−reset2**

reset counter 2 value to 0.

**−debug**

start debug mode. Print debug information.

OUTPUT
------
The k8055 dumps the result to stdout. 

**Output:** *(timestamp);(digital);(analog1);(analog2);(counter1);(counter2)*

Note: timestamp is the number of msec when data is read since program start

Example: *499;16;128;230;9;8*

    499 : Measure done 499 msec after program start
     16 : Digital input value is 10000 (I5=1, all other are 0) 
    128 : Analog 1 input value is 128 
    230 : Analog 2 input value is 230 
      9 : Counter 1 value is 9 
      8 : Counter 2 value is 8

EXAMPLES
--------
    $ k8055 −p:1 −d:147 −a1:25 −a2:203

this sets digital outputs 8,5,2,1 (binary 1001 0011), analog output 1 to
value 25 and analog output 2 to 203.

    $ k8055 −p:0 −num:5 −delay:500

this will read the inputs 5 times with 500ms interval producing
following output (input2/timer2 activade for 1 second): 

    20;0;109;190;0;0 
    513;0;109;190;0;0 
    1020;2;109;190;0;1 
    1515;2;109;190;0;1 
    2011;0;109;190;0;1

DIAGNOSTICS
-----------
The following diagnostics may be issued on stderr:

    Could not open the k8055 (port:1) 
    Please ensure that the device is correctly connected.

The k8055 board was not found at card address 1. Check the jumpers on
the board and verify your own board address. Otherwise you need to
verify that the board is found by your linux at an USB port. You can do
this by listing the usb devices in proc with `cat /proc/bus/usb/devices
| grep "S: Manufacturer=Velleman"` Leave out the grep part if you wish.

**Note:** the command can run slow for some reason since the k8055 sends
a huge amount of data to the USB bus...

Another problem might be if you use hotplug, then sometimes the HID
driver is loaded and reserves the device. Try with `rmmod hid` (if you
don’t have a usb keyboard/mouse!) and see if the board can be accessed.
If it worked then either disable hid in hotplug or setup hotplug
correctly...

BUGS
----
The debounce timer values are not accurate by the millisecond. The
0-7450ms (0-5000ms according to the velleman dll-manual but they
actually use 0-7450) is split along an exponetial scale in 255 sections.
The tolerance of the debounce time is within +-4% and down to +-2% in
200-4000ms range.

AUTHOR
------
Sven Lindberg \<k8055@k8055.mine.nu\>

SEE ALSO
--------
* http://libk8055.sourceforge.net

* * * * *
