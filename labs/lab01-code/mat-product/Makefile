CFLAGS = -Wall -g
CC 		 = gcc $(CFLAGS)

op_main: op_main.o outer_product.o free_matrix.o
	$(CC) -o op_main op_main.o outer_product.o free_matrix.o

op_main.o: mat.h op_main.c
	$(CC) -c op_main.c

outer_product.o: mat.h outer_product.c
	$(CC) -c outer_product.c

free_matrix.o: mat.h free_matrix.c
	$(CC) -c free_matrix.c

test: test_outer_product
	./test_outer_product

test_outer_product: test_outer_product.o outer_product.o mat.h 
	$(CC) -o  $@ $^

test_outer_product.o: test_outer_product.c mat.h
	$(CC) -c $<

clean:
	rm -f op_main test_outer_product *.o

