# Makefile for the genome handler library

.PHONY : install clean

GENOMELIB	= libgenome.a
HEADERS = genome.h genomelib.h
LIBPATH	= $(HOME)/lib
INCLUDEPATH = $(HOME)/include

CC 	= gcc
AR	= ar -rsc
OPT 	= -Wall -O2 -fomit-frame-pointer
# OPT 	= -Wall -g

GENOMELIBOBJS = genalloc.o readgnm.o  writegnm.o writeseq.o initgnm.o

INCLUDE = -I$(INCLUDEPATH)

CFLAGS 	= $(OPT) $(DBGDEF) $(DEFINE) $(INCLUDE)

$(GENOMELIB) : $(GENOMELIBOBJS)
	rm -f $(GENOMELIB)
	$(AR) $(GENOMELIB) $(GENOMELIBOBJS)

install : $(GENOMELIB)
	cp $(GENOMELIB) $(LIBPATH)
	cp $(HEADERS) $(INCLUDEPATH)

clean :
	rm -f *.o $(GENOMELIB) mdepend

mdepend :
	gcc $(CFLAGS) -MM $(GENOMELIBOBJS:%.o=%.c) > mdepend

include mdepend

