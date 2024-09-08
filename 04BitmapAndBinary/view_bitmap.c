/* Basic tool to view bitmap file in terminal */
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int cprint(char buffer[], int buflen, int red, int green, int blue) {
  return snprintf(buffer, buflen, "\033[48;2;%d;%d;%dm \033[0m", red, green, blue);
}

int hexBufferToDec(unsigned char init[], int ix1, int ix2) {
	int result = 0;
	int px = 0;
	for(int i = ix1; i < ix2; ++i) {
		/* For ex : E6 -> 6 [first] and E [second] */
		int first = init[i] & 0b00001111;
		int second = (init[i] & 0b11110000) >> 4;
		int resultFirst = pow(16,px);
		int resultSecond = pow(16,px+1);
		result += (first * resultFirst) + (second * resultSecond);
		px = px + 2;
	}
	return result;
}

int getFileSize(char* fileName) {
	/* Get fileSize of the provided bitmap file */
	unsigned int fileSize = 0;
	unsigned char init[6];

	FILE* ptr;
	ptr = fopen(fileName, "rb");
	fread(init, sizeof(init), 1, ptr);
	fclose(ptr);

	fileSize = hexBufferToDec(init, 2, 6);

	return fileSize;

}

int getOffsetAt(char* fileName) {
	/* Get Offset of actual pixel data */
	unsigned int offsetAt = 0;
	unsigned char init[14];

	FILE* ptr;
	ptr = fopen(fileName, "rb");
	fread(init, sizeof(init), 1, ptr);
	fclose(ptr);

	offsetAt = hexBufferToDec(init, 10, 14);

	return offsetAt;
}

int getWidth(char* fileName) {
	/* Get Width (no. of columns) or (no. of color blocks) */
	unsigned int width = 0;
	unsigned char init[26];

	FILE* ptr;
	ptr = fopen(fileName, "rb");
	fread(init, sizeof(init), 1, ptr);
	fclose(ptr);

	width = hexBufferToDec(init, 18, 22);

	return width;
}

int getHeight(char* fileName) {
	/* Get Height (no. of rows) */
	unsigned int height = 0;
	unsigned char init[26];

	FILE* ptr;
	ptr = fopen(fileName, "rb");
	fread(init, sizeof(init), 1, ptr);
	fclose(ptr);

	height = hexBufferToDec(init, 22, 26);

	return height;
}

void displayBmpMultipleOf4(int height, int width, unsigned char pixelData[], int pixelDataSize) {
	/* Displaying Blocks */
	int rgb = 3;
	int bytesPerCol = width*rgb;
	int start = pixelDataSize - bytesPerCol;

	for(int i = 1; i <= height; ++i) {
		for(int j = 0; j < width; ++j) {
			int red = pixelData[start+2];
			int green = pixelData[start+1];
			int blue = pixelData[start];

			//printf(" [ %x , %x , %x ]", blue, green, red);

			char buff[100];
			cprint(buff, 100, red, green, blue);
			cprint(buff, 100, red, green, blue);

			printf(buff);
			printf(buff);

			start += 3;
	}
		start = pixelDataSize - (bytesPerCol*i);
	    printf("\033[0;m");
		printf("\n");
	}
}

void displayBmpNotMultipleOf4(int height, int width, unsigned char pixelData[], int pixelDataSize) {
	/* Displaying Blocks */
	int rgb = 3;
	// to take care of padding
	// if width = 5 => 5*3 = 15 => need to add padding of 1 to make it a multiple of 4
	// if width = 30 => 30*3 = 90 => need to add 2 padding to make it a multiple of 4
	int paddingOfBytes = abs(4 - (width*3) % 4);
	int bytesPerCol = width*rgb + paddingOfBytes;

	int start = pixelDataSize - bytesPerCol;

	for(int i = 2; i <= height + 1; ++i) {
		for(int j = 0; j < width; ++j) {
			int red = pixelData[start+2];
			int green = pixelData[start+1];
			int blue = pixelData[start];

			//printf(" [ %x , %x , %x ]", blue, green, red);

			char buff[100];
			cprint(buff, 100, red, green, blue);
			cprint(buff, 100, red, green, blue);

			printf(buff);
			printf(buff);

			start += 3;
	}
		start = pixelDataSize - (bytesPerCol*i);
	    printf("\033[0;m");
		printf("\n");
	}
}


int main(int argc, char** argv) {
	if(argc < 2) {
		printf("[!] Use ./binary [filename]");
		return -1;
	}

	/* Get fileSize */
	char* fileName = argv[1];
	int fileSize = getFileSize(fileName);
	int offsetAt = getOffsetAt(fileName);

	int pixelDataSize = (fileSize) - offsetAt;
	unsigned char bmpFile[fileSize];

	FILE* ptr;
	ptr = fopen(fileName, "rb");
	fread(bmpFile, sizeof(bmpFile), 1, ptr);
	fclose(ptr);

	/* Get relevant pixelData */
	unsigned char pixelData[pixelDataSize];
	int ix = 0;
	for(int i = offsetAt; i < fileSize; ++i) {
		pixelData[ix] = bmpFile[i];
		ix++;
	}

	/* Displaying Blocks */
	int height = getHeight(fileName);
	int width = getWidth(fileName);

	if(width % 4 == 0) {
		displayBmpMultipleOf4(height, width, pixelData, pixelDataSize);
	} else {
		displayBmpNotMultipleOf4(height, width, pixelData, pixelDataSize);
	}

	return 0;
}