CPPFLAGS = -Wall -O3 -lm -w -std=c11
PROGRAMS = exact appro vote

all: $(PROGRAMS)

exact: exact.c dataloader.h networkloader.h common.h
	gcc -o exact exact.c $(CPPFLAGS)

appro: appro.c dataloader.h networkloader.h common.h
	gcc -o appro appro.c $(CPPFLAGS)

vote: vote.c dataloader.h networkloader.h common.h
	gcc -o vote vote.c $(CPPFLAGS)

clean:
	-rm -f *.o $(PROGRAMS)