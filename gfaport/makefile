.PHONY : install

LIBPATH = $(HOME)/lib
INCLUDEPATH = $(HOME)/include

GFAPORT	= libgfaport.a
GFAHEADERS = gfaport.h
CC	= gcc
AR	= ar -rsc
OPT	= -Wall -O2 -fomit-frame-pointer

CFLAGS	= $(INCLUDE) $(DEFINE) $(OPT)

OBJS 	= inp_bool.o inp_slng.o inp_ulng.o inp_dbl.o inp_str.o rnd.o bas_int.o rc_inter.o

$(GFAPORT): $(OBJS)
	rm -f $(GFAPORT)
	$(AR) $(GFAPORT) $(OBJS)

install : $(GFAPORT)
	cp $(GFAPORT) $(LIBPATH)
	cp $(GFAHEADERS) $(INCLUDEPATH)

clean :
	rm -f $(GFAPORT) $(OBJS) mdepend

mdepend :
	gcc $(CFLAGS) -MM $(OBJS:%.o=%.c) > mdepend

include mdepend

