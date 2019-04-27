CFLAGS = -Wall -g
CC     = gcc $(CFLAGS)

PROGRAMS = http_get http_get_ssl

all : $(PROGRAMS)

%.o : %.c
	$(CC) -c $<

http_get : http_get.o
	$(CC) -o $@ $^

http_get_ssl : http_get_ssl.o
	$(CC) -o $@ $^ -lssl -lcrypto

clean :
	rm -f *.o $(PROGRAMS)
