#include <stdio.h>

void displayString(char string[]) {
	printf("ASCII:");
	for(char* i = string; *i != 0x00; ++i) {
		printf("%8c", *i);
	}
	printf("\n");

	printf("Hex: ");
	for(char* i = string; *i != 0x00; ++i) {
		printf("%8x", *i);
	}
	printf("\n");

	printf("Dec: ");
	for(char* i = string; *i != 0x00; ++i) {
		printf("%8d", *i);
	}
	printf("\n");

	printf("Bin: ");
	for(char* i = string; *i != 0x00; ++i) {
		for(int j = 7; j >= 0; --j) {
			int bit = (*i >> j) & 0b00000001;
			printf("%u", bit);
		}
		printf("  ");
	}
	printf("\n");
}

int main(int argc, char** argv) {
	if(argc < 2) {
		printf("[!] Error :: ");
		printf(argv[0]);
		printf(" string");
	} else {
		char* string = argv[1];
		displayString(string);
	}
	return 0;
}