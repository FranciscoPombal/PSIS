all: prog1.c test1.c test2.c test.h
	gcc -Wall -Wextra -c prog1.c
	gcc -Wall -Wextra -c test2.c
	gcc -Wall -Wextra prog1.o test2.o -o prog1

.PHONY: clean
clean:
	rm -f *.o prog1
