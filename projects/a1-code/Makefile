CFLAGS = -Wall -g
CC 		 = gcc $(CFLAGS)
DEPS = commando.h
OBJ = cmd.o cmdcol.o util.o commando.o

TDEPS = test_utils.h test_commando.sh test_commando_data.sh standardize_pids
TOBJ = test_utils.o test_functions.o

.PHONY: clean, test-commando


commando: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^
	

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<
	

test-functions: $(TOBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c $(TDEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

test-commando: test-functions
	./test_commando.sh

clean:
	rm -f commando $(OBJ) $(TOBJ)

