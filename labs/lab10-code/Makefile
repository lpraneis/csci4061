CFLAGS = -Wall -g
CC     = gcc $(CFLAGS)

PROGRAMS =  em_server_mq         em_client_mq \
	    em_server_fifo       em_client_fifo \
	    faulty_em_server_mq  faulty_em_client_mq \


all : $(PROGRAMS)


em_server_mq : em_server_mq.c
	$(CC) -o $@ $^ -lrt

em_client_mq : em_client_mq.c
	$(CC) -o $@ $^ -lrt

em_server_fifo : em_server_fifo.c
	$(CC) -o $@ $^

em_client_fifo : em_client_fifo.c
	$(CC) -o $@ $^

faulty_em_server_mq : faulty_em_server_mq.c
	$(CC) -o $@ $^ -lrt

faulty_em_client_mq : faulty_em_client_mq.c
	$(CC) -o $@ $^ -lrt

# em_server_mq_busy : em_server_mq_busy.c
# 	$(CC) -o $@ $^ -lrt

clean :
	rm -f *.o *.fifo $(PROGRAMS)



