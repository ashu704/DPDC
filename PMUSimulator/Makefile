 # ----------------------------------------------------------------------------- 
 # Makefile for building PMU Simulator
 # DPDC - Phasor Data Concentrator
 # ----------------------------------------------------------------------------- 


SHELL = /bin/sh
prefix = /usr/local
includedir = /usr/src/include

INSTALL = /usr/bin/install -c
INSTALL_PREFIX = /usr/local/
CREATE_DIR = /usr/local/share/
DESKTOP_DIR = /usr/share/applications

FILE1 = pmu.glade
FILE2 = PmuLogo.png
FILE3 = pmu_background_img.png
FILE4 = PmuLogo1.png
FILE5 = logo.png
FILE6 = PMU.desktop
#FILE7 = DataDir

CC = gcc
LIBS = -lpthread -lm -lrt
LDLIBS=`pkg-config --cflags --libs gtk+-3.0`
CFLAGS=`pkg-config --cflags --cflags gtk+-3.0`

SOURCES = \
	pmu.c \
	PmuGui.c PmuGui.h \
	function.c function.h \
	ServerFunction.c ServerFunction.h \
	CfgFunction.c CfgFunction.h \
	CfgGuiFunctions.c CfgGuiFunctions.h \

OBJECTS =  pmu.c PmuGui.o function.o ServerFunction.o CfgFunction.o CfgGuiFunctions.o 
CLEAN_OBJECTS = PmuGui.o function.o ServerFunction.o CfgFunction.o CfgGuiFunctions.o

PROGRAM = PMU

COMPILE = $(CC) 

$(PROGRAM): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDLIBS) $(LIBS) -o $@

%.o: %.c
	$(COMPILE) $(CFLAGS) -c $<

install: $(PROGRAM)
	#gtk-builder-convert pmu.glade pmu.ui
	mkdir -p $(CREATE_DIR)PMU/images
	cp -f $(FILE1) $(CREATE_DIR)PMU/$(FILE1)
	cp -f images/$(FILE2) $(CREATE_DIR)PMU/images/$(FILE2)
	cp -f images/$(FILE3) $(CREATE_DIR)PMU/images/$(FILE3)
	cp -f images/$(FILE4) $(CREATE_DIR)PMU/images/$(FILE4)
	cp -f images/$(FILE5) $(CREATE_DIR)PMU/images/$(FILE5)
	cp -f images/$(FILE6) $(DESKTOP_DIR)/$(FILE6)
	cp $(PROGRAM) $(INSTALL_PREFIX)bin/$(PROGRAM)
	mkdir -p $(HOME)/DPDC/DataDir
	mkdir -p $(HOME)/DPDC/DataDir/50Hz_measurement_files
	mkdir -p $(HOME)/DPDC/DataDir/60Hz_measurement_files
	cp -f -r ./DataDir/50Hz_measurement_files/* $(HOME)/DPDC/DataDir/50Hz_measurement_files	#-i -n for Debian based Linux 
	cp -f -r ./DataDir/60Hz_measurement_files/* $(HOME)/DPDC/DataDir/60Hz_measurement_files
	chmod 777 $(HOME)/DPDC/
#	rm -f $(CLEAN_OBJECTS) $(PROGRAM)

uninstall:
	rm -f $(INSTALL_PREFIX)bin/$(PROGRAM)
	rm -rf $(CREATE_DIR)PMU/
	rm -rf $(DESKTOP_DIR)/$(FILE6)
	rm -f $(CLEAN_OBJECTS) $(PROGRAM)

clean:
	rm -f $(CLEAN_OBJECTS) $(PROGRAM)
