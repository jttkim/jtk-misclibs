# Makefile for the phylogenetic tree library

.PHONY : install

PTLIB	= libptlib.a
PTHEADERS = ptdefs.h pttypes.h ptlib.h
LIBPATH	= $(HOME)/lib
INCLUDEPATH = $(HOME)/include

CC 	= gcc
AR	= ar -rsc
# OPT 	= -Wall -O2 -fomit-frame-pointer
OPT 	= -Wall -g

PTLIBOBJS = addnode.o  freetree.o numleavs.o prntstuf.o readtree.o \
            rndbtree.o sortleav.o treedist.o ps_tree.o  \
            treehght.o treeinit.o setedges.o lendistr.o \
	    maxedgc.o  maxnlen.o  leafattr.o ptdist.o   \
	    leafdist.o leafset.o  maxlngth.o mergshrt.o setthick.o \
	    inf2thck.o cnt2thck.o tre2thck.o constlen.o setminl.o

INCLUDE = -I$(INCLUDEPATH)
LIBS	= $(JKLIBS) $(STDLIBS)

CFLAGS 	= $(OPT) $(DBGDEF) $(DEFINE) $(INCLUDE)

$(PTLIB) : $(PTLIBOBJS)
	rm -f $(PTLIB)
	$(AR) $(PTLIB) $(PTLIBOBJS)

install : $(PTLIB)
	cp $(PTLIB) $(LIBPATH)
	cp $(PTHEADERS) $(INCLUDEPATH)


clean :
	rm -f $(PTLIBOBJS) $(PTLIB) mdepend core

mdepend :
	$(CC) $(INCLUDE) -MM $(PTLIBOBJS:%.o=%.c) > mdepend

include mdepend

