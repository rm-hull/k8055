k8055
=====
Velleman k8055 linux driver and GUI sources for the Raspberry Pi

![Velleman k8055 board](https://raw.github.com/rm-hull/k8055/master/k8055.jpg)

From [libk8055.sourceforge.net](libk8055.sourceforge.net):

>"This software allows access to Velleman's K8055 card. This software was
developed to replace all other half-complete softwares for the k8055 board
under Linux. The library is made from scratch with the same functions as
described in Velleman's DLL usermanual. The command line tool is developed from
Julien Etelain and Edward Nys code.

>The reason for writing another driver for Linux was to get the debounce time to
work correctly. Nobody seemed to have figured out how it works (nor did
Velleman care to explain) so I studied it and have it working now with +-4%
accuracy of the actual settime. Note that the Velleman's DLL doesn't even get
this close.

>Another reason was to make it simple to use by writing a library for the k8055,
libk8055. Just by using "#include <k8055.h>" and compiling with "-lk8055" flags
you have access to all the functions described in Velleman's DLL documentation.

>The library is now also available in python, thanks to Pjetur G. Hjaltason.
Included with the source and some examples as well!

>Developer : Sven Lindberg <k8055@mrbrain.mine.nu> (python by Pjetur G.
Hjaltason <pjetur@pjetur.net>)"

That site nor the software is no longer actively being maintained.

How to build & install
----------------------
Install pre-requisites as follows, and build:

    $ sudo apt-get install libusb-dev swig libwxgtk2.8-dev
    $ make all
    $ sudo make install
    $ make pylib k8055gui
    $ sudo make pylibinstall guiinstall

Usage
-----
Plug in the K8055 board, observe that the power LED is illuminated and at the command-line,
check that the device is recognised:

    $ $ dmesg | grep Velleman
    [ 4508.616155] usb 1-1.2: Manufacturer: Velleman 
    [ 4508.631260] hid-generic 0003:10CF:5500.0001: hiddev0,hidraw0: USB HID v1.00 Device [Velleman  USB K8055] on usb-bcm2708_usb-1.2/input0

Then invoke the **k8055** command with elevated privileges:

    $ sudo k8055 -d:147

This should switch on digital LEDs 1, 2, 5 and 8. See the [manual](https://github.com/rm-hull/k8055/blob/master/MANUAL.md) page for more details.

Alternatively, use [setuid](https://en.wikipedia.org/wiki/Setuid) sticky bits to always run k8055 with root permissions:

    $ chmod u+s /usr/local/bin/k8055

The GTK gui must also be executed with root permissions, but GTK doesn't allow setuid, so start the gui using:

    $ gksu k8055gui

And providing an X display is present, the following dialog will be displayed:

![k8055gui screenshot](https://raw.github.com/rm-hull/k8055/master/gui/k8055gui.png)

Bugs
----
The **k8055gui** does not appear to properly set the digital outs.

License
-------
[GPL](http://www.gnu.org/licenses/gpl.html)

References
----------
* http://www.velleman.eu/products/view/?country=be&lang=en&id=351346

* http://www.velleman.eu/downloads/0/user/usermanual_k8055_dll_uk.pdf

* http://george-smart.co.uk/wiki/Nokia_3310_LCD

* https://sites.google.com/site/vellemank8055/

