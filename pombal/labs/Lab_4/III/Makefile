all: debug normal optimized

debug: III.c
	gcc -Wall -Wextra -g III.c -o III_debug -lrt

normal: III.c
	gcc -Wall -Wextra III.c -o III_normal -lrt

optimized: III.c
	gcc -Wall -Wextra -O3 III.c -o III_optimized -lrt

time: all
	time ./III_debug
	time ./III_normal
	time ./III_optimized

.PHONY: clean
clean:
	rm -f III_*
