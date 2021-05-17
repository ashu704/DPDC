 # ----------------------------------------------------------------------------- 
 # Makefile for building DPDC			
 #
 # ----------------------------------------------------------------------------- 


SHELL = /bin/sh
prefix = /usr/local
includedir = /usr/src/include

INSTALL = /usr/bin/install -c
INSTALL_PREFIX = /usr/local/
CREATE_DIR = /usr/local/share/
DESKTOP_DIR = /usr/share/applications

FILE1 = DPDC.glade
FILE2 = dpdcLogo.png
FILE3 = background_img.png
FILE4 = dpdcLogo1.png
FILE5 = logo.png
FILE6 = DPDC.desktop

CC = gcc
LIBS = -lpthread
LDLIBS=`pkg-config --cflags --libs gtk+-3.0`
CFLAGS=-g `pkg-config --cflags --cflags gtk+-3.0`

SOURCES = \
	dpdc.c \
	recreate.c recreate.h \
	connections.c connections.h \
	new_pmu_or_pdc.c new_pmu_or_pdc.h \
	parser.c parser.h \
	dallocate.c dallocate.h \
	align_sort.c align_sort.h \
	dpdcGui.c dpdcGui.h 

OBJECTS =  dpdc.c recreate.o connections.o new_pmu_or_pdc.o parser.o dallocate.o align_sort.o dpdcGui.o 

CLEAN_OBJECTS =  recreate.o connections.o new_pmu_or_pdc.o parser.o dallocate.o align_sort.o dpdcGui.o

PROGRAM = DPDC

COMPILE = $(CC) 

$(PROGRAM): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDLIBS) $(LIBS) -o $@

%.o: %.c
	$(COMPILE) $(CFLAGS) -c $<

install: $(PROGRAM)
	#gtk-builder-convert DPDC.glade DPDC.xml
	#gtk-builder-convert DPDC.glade DPDC.ui
	mkdir -p $(CREATE_DIR)DPDC/images
	cp -f $(FILE1) $(CREATE_DIR)DPDC/$(FILE1)
	cp -f images/$(FILE2) $(CREATE_DIR)DPDC/images/$(FILE2)
	cp -f images/$(FILE3) $(CREATE_DIR)DPDC/images/$(FILE3)
	cp -f images/$(FILE4) $(CREATE_DIR)DPDC/images/$(FILE4)
	cp -f images/$(FILE5) $(CREATE_DIR)DPDC/images/$(FILE5)
	cp -f images/$(FILE6) $(DESKTOP_DIR)/$(FILE6)
	cp $(PROGRAM) $(INSTALL_PREFIX)bin/$(PROGRAM)
#	rm -f $(CLEAN_OBJECTS) $(PROGRAM)

uninstall:
	rm -f $(INSTALL_PREFIX)bin/$(PROGRAM)
	rm -rf $(CREATE_DIR)DPDC/
	rm -rf $(DESKTOP_DIR)/$(FILE6)
	rm -f $(CLEAN_OBJECTS) $(PROGRAM)

clean:
	rm -f $(CLEAN_OBJECTS) $(PROGRAM)
