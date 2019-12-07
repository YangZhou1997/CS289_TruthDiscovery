CPPFLAGS = -Wall -O3 -lm -w -std=c11
PROGRAMS = exact appro

all: $(PROGRAMS)

exact: exact.c dataloader.h networkloader.h common.h
	gcc -o exact exact.c $(CPPFLAGS)

appro: appro.c dataloader.h networkloader.h common.h
	gcc -o appro appro.c $(CPPFLAGS)

clean:
	-rm -f *.o $(PROGRAMS)