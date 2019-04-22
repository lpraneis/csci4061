CFLAGS = -Wall -g
CC     = gcc $(CFLAGS)

PROGRAMS = check_spelling_busy check_spelling_condvar

all : $(PROGRAMS)

%.o : %.c			#default rule to create .o files
	$(CC) -c $<

check_spelling_busy : check_spelling_busy.o
	$(CC) -o $@ $^ -lpthread

check_spelling_condvar : check_spelling_condvar.o
	$(CC) -o $@ $^ -lpthread

clean :
	rm -f *.o $(PROGRAMS)



