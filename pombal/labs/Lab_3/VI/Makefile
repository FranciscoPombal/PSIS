ARGS=test.txt

all: VI-1 VI-2

VI-1: VI-1.c
	gcc -Wall -Wextra -g VI-1.c -o VI-1

VI-2: VI-2.c
	gcc -Wall -Wextra -g VI-2.c -o VI-2

run: all
	./VI-1 $(ARGS) && echo "\n==========\n==========\nNEXT PROGRAM\n==========\n==========\n" && ./VI-2 $(ARGS)

.PHONY: clean
clean:
	rm -f VI-1 VI-2
