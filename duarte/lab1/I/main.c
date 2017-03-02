#include <stdio.h>
#include <string.h>

int main(int argc, char **argv){
	char result_str[1000];
	result_str[0] = '\0';
	int i;

	for(i=1; i < argc; i++){
		strncat(result_str, argv[i], 20);
	}

	printf("\nresult_str: %s\n\n", result_str);

	return 0;
}
