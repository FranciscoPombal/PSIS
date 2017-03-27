all: lab5-1 verifier

lab5-1: lab5-1.c
	gcc -Wall -Wextra -g lab5-1.c -o lab5-1

verifier: verifier.c
	gcc -Wall -Wextra -g verifier.c -o verifier

run: lab5-1
	./lab5-1

run_verifier: verifier
	./verifier > correct_results.txt

.PHONY: clean
clean:
	rm -f lab5-1 verifier correct_results.txt
