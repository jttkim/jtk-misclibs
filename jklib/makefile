# Makefile for Jan's misc. utils library

.PHONY : install

JKLIB	= libjklib.a
JKHEADERS = jklib.h

CC 	= gcc
AR	= ar -rsc
OPT 	= -Wall -O2 -fomit-frame-pointer

include jkobjs

INCLUDEPATH	= $(HOME)/include
LIBPATH	= $(HOME)/lib

INCLUDE = -I$(INCLUDEPATH)

CFLAGS 	= $(OPT) $(DBGDEF) $(DEFINE) $(INCLUDE)

LIBS	= $(LIBPATH) $(LIBS)

$(JKLIB) : $(JKLIBOBJS)
	rm -f $(JKLIB)
	$(AR) $(JKLIB) $(JKLIBOBJS)

install : $(JKLIB) 
	cp $(JKLIB) $(LIBPATH)
	cp $(JKHEADERS) $(INCLUDEPATH)

clean :
	rm -f $(JKLIBOBJS) $(JKLIB) mdepend core

mdepend :
	gcc $(INCLUDE) -MM $(JKLIBOBJS:%.o=%.c) > mdepend

include mdepend

