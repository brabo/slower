CC=gcc

CFLAGS=-I. --std=gnu99 -I./include

SLOW_OBJ=src/slow.o

LDFLAGS=
LDLIBS=

slow: $(SLOW_OBJ)
	$(CC) -o $@ $(SLOW_OBJ) $(CFLAGS) $(LDFLAGS) $(LDLIBS)
	rm -f $(SLOW_OBJ)

all: slow

clean:
	rm -f src/*.o slow
