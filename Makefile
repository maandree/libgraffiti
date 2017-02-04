.POSIX:

CONFIGFILE = config.mk
include $(CONFIGFILE)

.SUFFIXES:
.SUFFIXES: .o .c

HDR =\
	graffiti.h\
	utf.h

LIBGRAFFITI = libgraffiti.a
LIBGRAFFITISRC =\
	libgraffiti/getgraffito.c

LIBUTF = libutf.a
LIBUTFSRC =\
	libutf/fgetrune.c\
	libutf/fputrune.c\
	libutf/isalnumrune.c\
	libutf/isalpharune.c\
	libutf/isblankrune.c\
	libutf/iscntrlrune.c\
	libutf/isdigitrune.c\
	libutf/isgraphrune.c\
	libutf/isprintrune.c\
	libutf/ispunctrune.c\
	libutf/isspacerune.c\
	libutf/istitlerune.c\
	libutf/isxdigitrune.c\
	libutf/lowerrune.c\
	libutf/rune.c\
	libutf/runetype.c\
	libutf/upperrune.c\
	libutf/utf.c\
	libutf/utftorunestr.c

LIBGRAFFITIOBJ = $(LIBGRAFFITISRC:.c=.o)
LIBUTFOBJ = $(LIBUTFSRC:.c=.o)

LIB = $(LIBGRAFFITI) $(LIBUTF)

all: $(LIB)

$(LIBGRAFFITI): $(LIBGRAFFITIOBJ)
	$(AR) -rc $@ $?
	$(AR) -s $@

$(LIBUTF): $(LIBUTFOBJ)
	$(AR) -rc $@ $?
	$(AR) -s $@

.o:
	$(CC) $(LDFLAGS) -o $@ $< $(LIB)

.c.o:
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ -c $<

clean:
	-rm -f $(LIB)

.PHONY:
	all clean
