# $Id: Makefile,v 1.3 2007/04/08 10:54:49 pjetur Exp $
#
#cc = gcc
#pylibdir = pyk8055
#libs = -lusb -L/usr/lib -lm

# We have to build extension "inplace" if We're to 
# run samples locally without installing

all: libk8055.i ../libk8055.c
	python setup.py build_ext --inplace
	python setup.py build

# Or in plain language
#	swig -python libk8055.i
#	cc -fpic -O2 -c ../libk8055.c -o ./libk8055.o
#	cc -fpic -O2 -c libk8055_wrap.c -I /usr/include/python2.4/ -o libk8055_wrap.o
#	ld -shared libk8055.o libk8055_wrap.o -lusb -o _pyk8055.so
##	mv -f pyk8055.py __init__.py

.PHONY:	kdemo

kdemo: k8055qt.py frmk8055qt.ui frmk8055qt.ui.h
	pyuic frmk8055qt.ui -o frmk8055qt.py

# install will both make build and install 
install:
	python setup.py install

# Remove all swig generated files
clean:
	python setup.py clean --all
	rm -f *.o _pyk8055.so libk8055_wrap.c pyk8055.py pyk8055.pyc

uninstall:
	python setup.py uninstall 
	#rm -f /usr/lib/python-2.4/site-packages/pyk8055.py /usr/lib/python-2.4/site-packages/_pyk8055.so
