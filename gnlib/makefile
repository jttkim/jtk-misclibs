# Makefile for the genealogy tree library

.PHONY : install all

GNLIB	= libgnlib.a
GNHEADERS = gndefs.h gntypes.h gnlib.h
LIBPATH	= $(HOME)/lib
INCLUDEPATH = $(HOME)/include

CC 	= gcc
CCXX	= g++
AR	= ar -rsc
OPT 	= -Wall -O2 -fomit-frame-pointer
# OPT 	= -Wall -g

include objects

INCLUDE = -I$(INCLUDEPATH)
# DBGDEF	= -DGNLIB_DEBUG
CFLAGS 	= $(OPT) $(DBGDEF) $(DEFINE) $(INCLUDE)

$(GNLIB) : $(GNLIBOBJS)
	rm -f $(GNLIB)
	$(AR) $(GNLIB) $(GNLIBOBJS)

all : example gnhx gnps gnps_tragic

example : example.o $(GNLIB) indiv.o sim.o
	$(CCXX) example.o indiv.o sim.o -o example -L. -lgnlib

inout : inout.o $(GNLIB) indiv.o sim.o
	$(CCXX) inout.o indiv.o sim.o -o inout -L. -lgnlib

gnhx : gnhx.o $(GNLIB)
	$(CCXX) gnhx.o -o gnhx -L. -lgnlib

gnps : gnps.o $(GNLIB)
	$(CCXX) gnps.o -o gnps -L. -lgnlib

gnps_tragic : gnps_tragic.o $(GNLIB)
	$(CCXX) gnps_tragic.o -o gnps_tragic -L. -lgnlib

installprg : example gnhx gnps gnps_tragic strattree
	cp gnhx gnps gnps_tragic strattree $(HOME)/bin

install : $(GNLIB)
	cp $(GNLIB) $(LIBPATH)
	cp $(GNHEADERS) $(INCLUDEPATH)

clean :
	rm -f $(GNLIBOBJS) $(GNLIB) mdepend core

mdepend : objects
	gcc -MM *.c > mdepend
	g++ -MM *.cxx >> mdepend

include mdepend

%.o : %.cxx
	$(CCXX) -c $(CFLAGS) $*.cxx -o $*.o

