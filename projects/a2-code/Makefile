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

## TEST TARGETS
TEST_PROGRAMS = test_blather.sh test_blather_data.sh normalize.awk cat_sig.sh filter-semopen-bug.awk 

test : test-blather

make test-blather : bl_client bl_server $(TEST_PROGRAMS)
	chmod u+rx $(TEST_PROGRAMS)
	./test_blather.sh

clean-tests :
	cd test-data && \
	rm -f *.* 
