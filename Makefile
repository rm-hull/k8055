# $Id: Makefile,v 1.5 2008/05/21 20:22:44 mr_brain Exp $
#
# Simple makefile for the libk8055 library
#
# change this to your distribution default
# usually /usr/local or /usr
PREFIX = /usr/local
CC = gcc
EXEC = k8055
VERSION =0.4.1
BINDIR = $(PREFIX)/bin
LIBDIR = $(PREFIX)/lib
PYLIBDIR = pyk8055
GUIDIR = gui
INCLUDEDIR = $(PREFIX)/include
MANDIR = $(PREFIX)/man/man1
OBJS = main.o libk8055.o
CFLAGS = -O2 -DDAEMON -DVERSION='"$(VERSION)"' -Wall
LIBS = -lusb -L/usr/lib -lm
LINK = ln

libk8055.so.$(VERSION): libk8055.o
	$(CC) -o libk8055.so.${VERSION} $(LIBS) -shared libk8055.o
	-$(LINK) -sf libk8055.so.${VERSION} libk8055.so

libk8055.o: libk8055.c
	$(CC) $(CFLAGS) -c -g -fPIC libk8055.c

libk8055.a: libk8055.o
	ar rvs libk8055.a libk8055.o

libk8055.c: k8055.h

all: k8055_prog libk8055.so.${VERSION} libk8055.a

.PHONY: k8055_prog

k8055_prog: $(OBJS)
	$(CC) $(OBJS) -o $(EXEC) $(LIBS)
	# strip $(EXEC)

pylib: $(PYLIBDIR)/libk8055.i libk8055.c
	export VERSION=$(VERSION); $(MAKE) -C $(PYLIBDIR)

k8055gui: libk8055.so.$(VERSION)
	$(MAKE) -C $(GUIDIR)

clean:	
	rm -f *.o libk8055.so libk8055.so.$(VERSION) $(EXEC) libk8055.a
	@$(MAKE) -C $(PYLIBDIR) clean
	@$(MAKE) -C $(GUIDIR) clean

install: k8055_prog libk8055.so.$(VERSION)
	cp -f $(EXEC) $(BINDIR)/
	if !(test -d $(LIBDIR)); then \
	  mkdir -p $(LIBDIR); \
	fi
	if !(test -d $(INCLUDEDIR)); then \
	  mkdir -p $(INCLUDEDIR); \
	fi
	rm -f $(LIBDIR)/libk8055* 
	cp -Pf lib* $(LIBDIR)
	cp -f k8055.h $(INCLUDEDIR)
	if !(test -d $(MANDIR)); then \
	  mkdir -p $(MANDIR); \
	fi
	cp -f man/k8055.1.gz $(MANDIR)/

pyinstall: $(PYLIBDIR)/libk8055.i
	@$(MAKE) -C $(PYLIBDIR) install

guiinstall: $(GUIDIR)/k8055gui
	@$(MAKE) -C $(GUIDIR) install

uninstall:
	rm -f $(BINDIR)/$(EXEC) $(LIBDIR)/libk8055* $(INCLUDEDIR)/k8055.h

test: k8055_prog
	-@killall $(EXEC)
	./$(EXEC)
