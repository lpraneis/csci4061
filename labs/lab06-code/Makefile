CFLAGS = -Wall -g -Og
CC     = gcc $(CFLAGS)

all: make_dept_directory print_department_read print_department_mmap

print_department_read : print_department_read.c department.h
	$(CC) -o $@ $^		

print_department_mmap : print_department_mmap.c department.h
	$(CC) -o $@ $^		

make_dept_directory : make_dept_directory.c department.h
	$(CC) -o $@ $^		

datfile : make_dept_directory
	./make_dept_directory cse_depts.dat

clean:
	rm -f *.o  print_department_read print_department_mmap make_dept_directory
