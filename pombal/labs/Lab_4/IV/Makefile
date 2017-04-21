all: gen_random count_odd count_even

gen_random: gen_random.c
	gcc -Wall -Wextra -g gen_random.c -o gen_random -lrt

count_odd: count_odd.c
	gcc -Wall -Wextra -g count_odd.c -o count_odd -lrt

count_even: count_even.c
	gcc -Wall -Wextra -g count_even.c -o count_even -lrt

run: all
	./gen_random &
	./count_odd &
	./count_even

.PHONY: clean
clean:
	rm -f gen_random count_even count_odd
