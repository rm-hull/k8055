# Indicates compilator to use
CC      = g++

# Specifies compilator options
CFLAGS  = `wx-config --cxxflags`
LDFLAGS = `wx-config --libs`
LDLIBS  = -lusb -lm -lk8055
PREFIX = /usr/local
BINDIR = $(PREFIX)/bin

# Files extensions .cpp, .o
SUFFIXES = .cpp .o 
.SUFFIXES: $(SUFFIXES) .

# Name of the main program
PROG  = k8055gui

# Object files .o necessary to build the main program
OBJS  = MyApp.o MyFrame.o
 
all: $(PROG)

# Compilation and link
$(PROG): $(OBJS)
	$(CC) $(LDFLAGS) -o $(PROG) $(OBJS) $(LDLIBS)

.cpp.o:
	$(CC)   $(CFLAGS) -c $< -o $@

clean:
	-rm -f $(PROG)
	-rm -f *.o

install:
	cp -f $(PROG) $(BINDIR)

