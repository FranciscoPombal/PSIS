all: lab5-5-gen_random lab5-5-count_odd lab5-5-count_even

lab5-5-gen_random: lab5-5-gen_random.c
	gcc -Wall -Wextra -g lab5-5-gen_random.c -o lab5-5-gen_random -lrt

lab5-5-count_odd: lab5-5-count_odd.c
	gcc -Wall -Wextra -g lab5-5-count_odd.c -o lab5-5-count_odd -lrt

lab5-5-count_even: lab5-5-count_even.c
	gcc -Wall -Wextra -g lab5-5-count_even.c -o lab5-5-count_even -lrt

run: all
	./lab5-5-gen_random

.PHONY: clean
clean:
	rm -f lab5-5-gen_random lab5-5-count_even lab5-5-count_odd
