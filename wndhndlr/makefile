# Makefile for the window handler library

.PHONY : install

WHNDLRLIB	= libwhndlr.a
PTHEADERS = wndhndlr.h wndincl.h
LIBPATH	= $(HOME)/lib
INCLUDEPATH = $(HOME)/include

CC 	= gcc
AR	= ar -rsc
OPT 	= -Wall -O2 -fomit-frame-pointer
STDLIBS	= -lX11

WHNDLRLIBOBJS = x_init.o x_event.o create_w.o wndglobl.o map_win.o remove_w.o redr_all.o \
		mousebox.o

INCLUDE = -I$(INCLUDEPATH)

CFLAGS 	= $(OPT) $(DBGDEF) $(DEFINE) $(INCLUDE)

LIBS	= $(STDLIBS)

$(WHNDLRLIB) : $(WHNDLRLIBOBJS)
	rm -f $(WHNDLRLIB)
	$(AR) $(WHNDLRLIB) $(WHNDLRLIBOBJS)

whtest : $(WHNDLRLIB) whtest.o
	$(CC) whtest.o $(WHNDLRLIB) -o whtest $(LIBS)

install : $(WHNDLRLIB)
	cp $(WHNDLRLIB) $(LIBPATH)
	cp $(PTHEADERS) $(INCLUDEPATH)

clean :
	rm -f $(WHNDLRLIBOBJS) whtest.o $(WHNDLRLIB) mdepend core

mdepend :
	gcc $(INCLUDE) -MM $(WHNDLRLIBOBJS:%.o=%.c) > mdepend

include mdepend

