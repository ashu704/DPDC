 # ----------------------------------------------------------------------------- 
 # Makefile for building DBServer
 #
 # ----------------------------------------------------------------------------- 


SHELL = /bin/sh
prefix = /usr/local
includedir = /usr/src/include

INSTALL = /usr/bin/install -c
INSTALL_PREFIX = /usr/local

CC = gcc
LIBS = -lpthread -lm
LDLIBS=`mysql_config --cflags --libs`
CFLAGS=-Wall -g `mysql_config --cflags --libs`

SOURCES = \
	dbserver.c \
	recreate.c recreate.h \
	connections.c connections.h \
	parser.c parser.h \
	dallocate.c dallocate.h

OBJECTS =  dbserver.c recreate.o connections.o parser.o dallocate.o 

CLEAN_OBJECTS =  recreate.o connections.o new_pmu_or_pdc.o parser.o dallocate.o cfg.bin

PROGRAM = DBServer

COMPILE = $(CC) 

$(PROGRAM): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDLIBS) $(LIBS) -o $@

%.o: %.c
	$(COMPILE) $(CFLAGS) -c -w $<

clean:
	rm -f $(CLEAN_OBJECTS) $(PROGRAM)

install: $(PROGRAM)
	cp -f $(PROGRAM) $(INSTALL_PREFIX)/bin/$(PROGRAM)

uninstall:
	rm -f $(INSTALL_PREFIX)/bin/$(PROGRAM)
