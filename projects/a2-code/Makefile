CFLAGS = -Wall -g
LDFLAGS = -lpthread
CC 		 = gcc $(CFLAGS) 
DEPS = blather.h
SOBJ = server_funcs.o bl_server.o  util.o simpio.o
COBJ = server_funcs.o bl_client.o util.o simpio.o

TOBJ = server_funcs.o util.o simpio.o 

.PHONY: clean, test, clean-tests,  all, test-shell

# Normal Targets
all: bl_server bl_client

bl_server: $(SOBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

bl_client: $(COBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

simpio: simpio.c simpio_demo.c
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(LDFLAGS)

clean:
	rm -f simpio bl_server bl_client $(SOBJ) $(COBJ) $(TOBJ)
	rm -f *.fifo

# Testing Targets
test-binary: $(TOBJ)
	$(CC) -o $@ $^

test-shell: 
	./test-shell.sh



