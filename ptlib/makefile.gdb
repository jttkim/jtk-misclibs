# Makefile for the phylogenetic tree library

PTLIB	= libptlib.a
PTHEADERS = ptdefs.h pttypes.h ptlib.h
LIBPATH	= $(HOME)/debuglib
INCLUDEPATH = $(HOME)/include

CC 	= gcc
AR	= ar -rsc
OPT 	= -Wall -g
DEFINE	= -DMEMDEBUG
STDLIBS	= -lmalloc

PTLIBOBJS = addnode.o  freetree.o numleavs.o prntstuf.o readtree.o \
            rndbtree.o sortleav.o treedist.o ps_tree.o  \
            treehght.o treeinit.o setedges.o lendistr.o \
	    maxedgc.o  maxnlen.o  leafattr.o ptdist.o   \
	    leafdist.o leafset.o  maxlngth.o mergshrt.o setthick.o \
	    inf2thck.o cnt2thck.o tre2thck.o constlen.o

JKINCL	= -I$(HOME)/include
JKLIBS	= -L$(HOME)/lib -lmemdebug

INCLUDE = $(JKINCL)

CFLAGS 	= $(OPT) $(DBGDEF) $(DEFINE) $(INCLUDE)

LIBS	= $(JKLIBS) $(STDLIBS)

$(PTLIB) : $(PTLIBOBJS)
	rm -f $(PTLIB)
	$(AR) $(PTLIB) $(PTLIBOBJS)

install : $(PTLIB) inst_headers
	cp $(PTLIB) $(LIBPATH)
	touch install

inst_headers : $(PTHEADERS)
	cp $(PTHEADERS) $(INCLUDEPATH)
	touch inst_headers

clean :
	rm -f $(PTLIBOBJS) $(PTLIB) mdepend core

mdepend :
	$(CC) $(INCLUDE) -MM $(PTLIBOBJS:%.o=%.c) > mdepend

include mdepend



