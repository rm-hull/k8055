k8055
=====

Velleman k8055 linux driver sources for the Raspberry Pi

![Velleman k8055 board](master/k8055.jpg)

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

    $ sudo apt-get install libusb-dev swig
    $ make all pylib
    $ sudo make install pyinstall

Usage
-----
TODO

License
-------
[GPL](http://www.gnu.org/licenses/gpl.html)

