# See Makefile-commented for explanation
CFLAGS = -Wall -g 
CC     = gcc $(CFLAGS)

collcalc : collatz.o collatz_main.o
	$(CC) -o collcalc collatz.o collatz_main.o
	@echo collcalc is ready

collatz_main.o : collatz_main.c collatz.h
	$(CC) -c collatz_main.c

collatz.o : collatz.c collatz.h
	$(CC) -c $<

test : collatz_test
	./collatz_test

collatz_test : collatz_test.o collatz.o collatz.h
	$(CC) -o $@ $^

collatz_test.o : collatz_test.c collatz.h
	$(CC) -c $<

clean:
	rm -f collcalc collatz_test *.o

# realclean:
# 	@echo Removing objects and programs
# 	rm -f *.o collcalc collatz_test

