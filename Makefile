SRCS :=	getfreq.c \
	getsigs.c \
	init.c \
	off.c \
	on.c \
	setsamplerate.c \
	setfreq.c \
	setpwr.c \
	setsig.c \
	stat.c \
	sysoff.c \
	syson.c \


OBJS := $(SRCS:.c=.o) # list of objects
PROGS := $(SRCS:.c=)  # list of programs
MIRACLELIB = miraclelib.o # miraclelib object

CC = gcc

.PHONY: all
all: $(PROGS)

$(PROGS): % : %.o $(MIRACLELIB) 
	$(CC) $(LDFLAGS) -o $@ $@.o $(MIRACLELIB) -lm

$(OBJS): $(MIRACLELIB) 

# rm -f $(PROGS) *.elf *.gdb *.o

.PHONY: clean
clean:
	# rm -f *.elf *.gdb *.o
	 rm -f $(PROGS) *.elf *.gdb *.o
