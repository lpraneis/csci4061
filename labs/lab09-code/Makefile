CFLAGS = -Wall -g
CC     = gcc $(CFLAGS)

all : em_server em_client

em_server : em_server.c
	$(CC) -o $@ $^

em_client : em_client.c
	$(CC) -o $@ $^

clean :
	rm -f em_server em_client *.fifo


