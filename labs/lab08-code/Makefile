
all : sleep_print select_AB read_AB

sleep_print : sleep_print.c
	gcc -o $@ $^

select_AB : select_AB.c
	gcc -o $@ $^

read_AB : read_AB.c
	gcc -o $@ $^

select_AB_children_alive : select_AB_children_alive.c
	gcc -o $@ $^

clean :
	rm -f sleep_print select_AB read_AB

