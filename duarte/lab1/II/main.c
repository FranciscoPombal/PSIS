#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char **argv){
	char result_str[1000];
	result_str[0] = '\0';
	int i;

	/*printf("\n%s\n", argv[0]);
	printf("%s\n", argv[1]);
	printf("%s\n", argv[2]);
	printf("%s\n", argv[3]);*/

	for(i=1; i < argc; i++){
		strncat(result_str, argv[i], 20);
	}

	i=0;
	while(result_str[i]){
		result_str[i] = toupper(result_str[i]);
		i++;
	}

	printf("\nresult_str: %s\n\n", result_str);

	return 0;
}
