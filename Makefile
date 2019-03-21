CC = clang

CFLAGS = -g -Wall -pedantic

SRCS = pgrep.c dir.c grep.c

OBJS = $(SRCS:.c=.o)

MAIN = pgrep

LIBS =

.PHONY: clean

default: $(MAIN)

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) -o $(MAIN) $(OBJS) $(LIBS)

# this is a suffix replacement rule for building .o's from .c's
# it uses automatic variables $<: the name of the prerequisite of
# the rule(a .c file) and $@: the name of the target of the rule (a .o file)
# (see the gnu make manual section about automatic variables)
.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

test: $(MAIN)
	valgrind --leak-check=full ./$(MAIN) 4 int ./bitcoin

clean:
	-rm -f *.o
	-rm -f pgrep
