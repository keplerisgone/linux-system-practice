#include <stdio.h>

int main(int argc, char* argv[]){
	if ( argc != 2 ) {
		printf("usage: ./read-file [file name]\n");
		return 0;
	}
	
	FILE* pFile = fopen(argv[1], "r");
	if ( !pFile ) {
		printf("no file\n");
		return 0;
	}
	
	char str[100];
	while( fgets(str, 100, pFile )) {
		printf("%s", str);
	}
	
	fclose(pFile);

	return 0;
}
