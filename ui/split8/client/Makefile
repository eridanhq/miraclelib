#
# Makefile to build ECM Controller Client
#

GCCFLAGS :=\
-Wall -Wextra \
-Werror=return-type \
-Wvla -Wcast-qual -Wdangling-else -Winit-self -Werror=uninitialized \
-Werror=incompatible-pointer-types -Werror=array-bounds -Wshadow \
-Wduplicated-cond -Wnull-dereference -Wdangling-else -Waddress \
-Wint-in-bool-context -Winit-self \
-Werror=uninitialized \
-Wlogical-op -Werror=logical-op \
-Werror=sequence-point \
-Werror=missing-braces -Werror=write-strings -Werror=address -Werror=array-bounds \
-Werror=char-subscripts -Werror=enum-compare -Werror=implicit-int \
-Werror=empty-body -Werror=main -Werror=nonnull -Werror=parentheses \
-Werror=pointer-sign -Werror=ignored-qualifiers \
-Werror=missing-parameter-type \
-Wmissing-field-initializers \
-Wcast-align \
-Wduplicated-branches \
-Woverlength-strings \
-Wtrampolines \
-Wvariadic-macros \
-Werror=null-dereference \
-Wredundant-decls \
-Wno-declaration-after-statement 
# -std=gnu11 \
# -O3 \
# -Werror=unused-value \
# -Wformat \
# -Wformat-security \
# -Wformat-signedness \
# -Wpointer-arith \
# -Wmissing-declarations \
# -Werror=strict-prototypes \

SRCS :=	ecmctl.c


OBJS         := $(SRCS:.c=.o)   # list of objects
PROGS        := $(SRCS:.c=)     # list of programs
MIRACLELIB    = miraclelib.o    # miracle object
MIRACLELIBDEV = miraclelibdev.o # miracle dev object

CC = ${CROSS_COMPILE}gcc

.PHONY: all clean 
all: $(PROGS)

# .h dependency is not working correctly
# the double : % : for this target is explained here https://www.gnu.org/software/make/manual/html_node/Static-Usage.html#Static-Usage
# Ok so PROGS creates many rules, each are just the name of the binary
# the : % :  matches against this, effectively creating a variable %, which can only be used for prerequisites. The contents of this variable is the name of the binary with no extension or .
# the %.o in the prerequisites means that this target will be sensative to the .o object file (created below)

$(PROGS): % : %.o  *.h
	$(CC) $(GCCFLAGS) -I../ $(LDFLAGS) -o $@ $@.o -lm


$(OBJS): %.o : *.h %.c
	$(CC) $(GCCFLAGS) -I../ -g -c -o $*.o $*.c

#$(MIRACLELIBDEV): miraclelibdev.c miraclelibdev.h miraclelib.c miraclelib.h
#	$(CC) $(GCCFLAGS) -c -o miraclelibdev.o miraclelibdev.c

#$(MIRACLELIB): miraclelib.c miraclelib.h
#	$(CC) $(GCCFLAGS) -c -o miraclelib.o miraclelib.c


clean:
	# rm -f *.elf *.gdb *.o
	 rm -f $(PROGS) *.elf *.gdb *.o
