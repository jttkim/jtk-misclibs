# master makefile for the libraries of Jan T. Kim

.PHONY : install genomelib gnlib jklib gfaport ptlib wndhndlr clean

LIBPATH	= $(HOME)/lib
INCLUDEPATH = $(HOME)/include

# The LIBPREFIX and LIBPOSTFIX might be changed for systems other
# than Unix, e.g. Atari TOS systems.
LIBPREFIX  = lib
LIBPOSTFIX = .a

CC 	= gcc
CCXX	= g++
AR	= ar -rsc
OPT 	= -Wall -O6

# End of configurable part. Please contact me if any changes are
# necessary beyond this point

INCLUDEFILES = genome.h genomelib.h gfaport.h gndefs.h gnlib.h \
	gntypes.h jklib.h ptdefs.h ptlib.h pttypes.h wndhndlr.h \
	wndincl.h

LIBRARIES = $(LIBPREFIX)genome$(LIBPOSTFIX) \
	    $(LIBPREFIX)gfaport$(LIBPOSTFIX) \
	    $(LIBPREFIX)gnlib$(LIBPOSTFIX) \
	    $(LIBPREFIX)jklib$(LIBPOSTFIX) \
	    $(LIBPREFIX)ptlib$(LIBPOSTFIX) \
	    $(LIBPREFIX)whndlr$(LIBPOSTFIX)

MAKEARGS = CC='$(CC)' CCXX='$(CCXX)' AR='$(AR)' OPT='$(OPT)' LIBPATH='$(LIBPATH)' INCLUDEPATH='$(INCLUDEPATH)' LINDEVOL_BINDIR='$(LINDEVOL_BINDIR)'

install : genomelib gnlib jklib gfaport ptlib wndhndlr

genomelib : jklib
	$(MAKE) $(MAKEARGS) -C genomelib install

gnlib : jklib
	$(MAKE) $(MAKEARGS) -C gnlib install

jklib :
	$(MAKE) $(MAKEARGS) -C jklib install

gfaport:
	$(MAKE) $(MAKEARGS) -C gfaport install

ptlib : jklib
	$(MAKE) $(MAKEARGS) -C ptlib install

wndhndlr :
	$(MAKE) $(MAKEARGS) -C wndhndlr install

clean :
	$(MAKE) $(MAKEARGS) -C jklib clean
	$(MAKE) $(MAKEARGS) -C wndhndlr clean
	$(MAKE) $(MAKEARGS) -C genomelib clean
	$(MAKE) $(MAKEARGS) -C gnlib clean
	$(MAKE) $(MAKEARGS) -C gfaport clean
	$(MAKE) $(MAKEARGS) -C ptlib clean

