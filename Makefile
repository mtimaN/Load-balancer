CC=gcc
CFLAGS=-std=c99 -Wall -Werror -Wextra
LOAD=load_balancer
SERVER=server
LL = lists
LB_H = lb_helpers

.PHONY: build clean

build: tema2

tema2: main.o $(LOAD).o $(SERVER).o $(LL).o $(LB_H).o
	$(CC) $^ -o $@

main.o: main.c
	$(CC) $(CFLAGS) $^ -c

$(SERVER).o: $(SERVER).c $(SERVER).h
	$(CC) $(CFLAGS) $^ -c

$(LOAD).o: $(LOAD).c $(LOAD).h
	$(CC) $(CFLAGS) $^ -c

$(LL).o: $(LL).c $(LL).h
	$(CC) $(CFLAGS) $^ -c

$(LB_H).o: $(LB_H).c $(LB_H).h
	$(CC) $(CFLAGS) $^ -c

clean:
	rm -f *.o tema2 *.h.gch
