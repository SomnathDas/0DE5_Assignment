#include <stdio.h>

int main(int argc, char** argv) {
	unsigned char string[15] = {};

	string[0] = 0b01001101;
	string[1] = 0x79;
	string[2] = 0b00100000;
	string[3] = 0b01101110;
	string[4] = 0x61;
	string[5] = 0x6D;
	string[6] = 0b01100101;
	string[7] = 0x20;
	string[8] = 0x69;
	string[9] = 0x73;
	string[10] = 0b00101110;
	string[11] = '.';
	string[12] = 0b00101110;
	string[13] = 0b00100000;
	string[14] = 0x00;

	printf("[ix]  [hex]  [dec]  [chr]  [bin] \n");
	printf("____  _____  _____  _____  _________\n");
	for(int i = 0; i < 15; ++i) {
		printf("[%2u] %5x %5u %5c \t   ", i, string[i], string[i], string[i]);

		for(int j = 7; j >= 0; --j) {
			int bit = (string[i] >> j) & 0b00000001;
			printf("%u" , bit);
		}

		printf("\n");
	}

	return 0;
}