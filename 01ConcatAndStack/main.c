#include <stdio.h>

int isConcatSuccess(int check, int buffer_length) {
	return check >= 0 && check < buffer_length ? 1 : 0;
}

void fizzbuzz(char buffer[], int buffer_length, int top) {
	int i = 1;
	int check = 0;
	while(i < top) {
		if(i % 3 == 0 && i % 5 == 0) {
			check = snprintf(buffer, buffer_length, "%s", "Fizzbuzz, ");
		} else if(i % 3 == 0) {
			check = snprintf(buffer, buffer_length, "%s", "Fizz, ");
		} else if(i % 5 == 0) {
			check = snprintf(buffer, buffer_length, "%s", "Buzz, ");
		} else {
			check = snprintf(buffer, buffer_length, "%d, ", i);
		}

		if(!isConcatSuccess(check, buffer_length)) {
			return;
		}

		buffer += check;
		buffer_length -= check;

		i = i + 1;
	}

	buffer -= 2;
	check -= 2;
	snprintf(buffer, buffer_length, ".");

}

int main(int argc, char** argv) {
	char result[100] = {0};
	fizzbuzz(result, 100, 6);
	printf("%s", result);
	return 0;
}