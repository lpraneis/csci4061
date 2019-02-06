CLAGS = -Wall -g
CC 		 = gcc $(CFLAGS)
DEPS = commando.h
OBJ = cmd.o cmdcol.o util.o commando.o

.PHONY: clean, test, clean-tests, test-functions, test-commando

# Normal Targets
commando: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f commando $(OBJ) $(TOBJ)

# Testing Targets
test : test-functions test-commando

test-functions : test_functions
	@printf "Tests for correctness of functions\n"
	./test_functions
	@printf "\n"
	@printf "Valgrind with function tests\n"
	valgrind ./test_functions

test-commando : commando
	@printf "Tests for correctness of commando program\n"
	chmod u+x test_commando.sh standardize_pids
	./test_commando.sh

test_utils.o : test_utils.c test_utils.h
	$(CC) -c $<

test_functions.o : test_functions.c commando.h test_utils.h 
	$(CC) -c $<

test_functions : test_functions.o cmd.o cmdcol.o test_utils.o
	$(CC) -o $@ $^

clean-tests:
	rm -f test_functions test-data/*.tmp

