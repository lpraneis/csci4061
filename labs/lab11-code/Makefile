CFLAGS = -Wall -g
CC     = gcc $(CFLAGS)

PROGRAMS = worms_pthread 

all : $(PROGRAMS)

%.o : %.c			#default rule to create .o files
	$(CC) -c $<

worms_pthread : worms_pthread.o
	$(CC) -o $@ $^ -lpthread

clean :
	rm -f *.o $(PROGRAMS)



