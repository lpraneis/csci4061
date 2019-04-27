CFLAGS = -Wall -g
CC     = gcc $(CFLAGS)
# LIBS   = -lpthread

all : simple-server simple-client pause-server

simple-server : simple-server.c
	$(CC) -o $@ $^

simple-client : simple-client.c
	$(CC) -o $@ $^

pause-server : pause-server.c
	$(CC) -o $@ $^

clean :
	rm -f *.o simple-server simple-client pause-server
