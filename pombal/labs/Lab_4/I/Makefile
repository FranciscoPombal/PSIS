all: debug normal optimized

debug: lab4-1.c
	gcc -Wall -Wextra -g lab4-1.c -o lab4-1_debug

normal: lab4-1.c
	gcc -Wall -Wextra lab4-1.c -o lab4-1_normal

optimized: lab4-1.c
	gcc -Wall -Wextra -O3 lab4-1.c -o lab4-1_optimized

time: all
	time ./lab4-1_debug
	time ./lab4-1_normal
	time ./lab4-1_optimized

.PHONY: clean
clean:
	rm -f lab4-1_*
