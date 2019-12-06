CPPFLAGS = -Wall -O3 -lm -w -std=c11
PROGRAMS = exact

all: $(PROGRAMS)

exact: exact.c dataloader.h networkloader.h common.h
	gcc -o exact exact.c $(CPPFLAGS)

clean:
	-rm -f *.o $(PROGRAMS)