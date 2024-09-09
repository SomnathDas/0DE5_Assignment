#include <stdio.h>
#include <stdlib.h>

/* Constants */
const int BITMAP_FILE_HEADER_SIZE = 0xE;
const int BITMAP_INFO_HEADER_SIZE = 0x28;
const int BITS_PER_PIXEL = 0x18;
/* Constants */

typedef unsigned int color_t;

struct Bitmap {
	int height;
	int width;
	color_t *pixels;
};

struct Bitmap init_bitmap(int height, int width) {
	color_t *pixels = malloc(height * width * sizeof(color_t));
	struct Bitmap bitmap = {.width = width, .height = height, .pixels = pixels};
	return bitmap;
}

void setPixel(struct Bitmap bitmap, int x, int y, color_t color) {
	if(x < 0 || x >= bitmap.width || y < 0 || y >= bitmap.height) {
		return;
	}
	bitmap.pixels[y * bitmap.width + x] = color;
}

void free_bitmap(struct Bitmap bitmap) {
	free(bitmap.pixels);
	bitmap.pixels = NULL;
}

void writeBfType(FILE* file) {
	char magicBytes[2] = "BM";
	fwrite(magicBytes, sizeof(magicBytes), 1, file);
}

int calculateRowSize(int width) {
	int row_size = width * 3;
	if(row_size % 4 != 0) { // if padding is required
		row_size += 4 - (row_size % 4);	
	}
	return row_size;
}

int calculateImageSize(int width, int height) {
	int size = calculateRowSize(width) * height;
	return size;
}

void writeBfSize(FILE* file, int width, int height) {
	int size = BITMAP_FILE_HEADER_SIZE + BITMAP_INFO_HEADER_SIZE + calculateImageSize(width, height);
	fwrite(&size, 4, 1, file);
}

void writeBfReserved1(FILE* file) {
	int value = 0;
	fwrite(&value, 2, 1, file);
}

void writeBfReserved2(FILE* file) {
	int value = 0;
	fwrite(&value, 2, 1, file);
}

void writeBfOffBits(FILE* file) {
	int offsetAt = BITMAP_INFO_HEADER_SIZE + BITMAP_FILE_HEADER_SIZE;
	fwrite(&offsetAt, 4, 1, file);
}

void writeBiSize(FILE* file) {
	int header_size = BITMAP_INFO_HEADER_SIZE;
	fwrite(&header_size, 4, 1, file);
}

void writeBiWidth(FILE* file, int width) {
	int size = width;
	fwrite(&size, 4, 1, file);
}

void writeBiHeight(FILE* file, int height) {
	int size = height;
	fwrite(&size, 4, 1, file);
}

void writeBiPlanes(FILE* file) {
	int planes = 1;
	fwrite(&planes, 2, 1, file);
}

void writeBiBitCount(FILE* file) {
	int count = BITS_PER_PIXEL;
	fwrite(&count, 2, 1, file);
}

void writeBiCompression(FILE* file) {
	int compressionMethod = 0;
	fwrite(&compressionMethod, 4, 1, file);
}

void writeBiSizeImage(FILE* file, int width, int height) {
	int size = calculateImageSize(width, height);
	fwrite(&size, 4, 1, file);
}

void writeBiXPelsPerMeter(FILE* file) {
	int print_resolution = 0;
	fwrite(&print_resolution, 4, 1, file);
}

void writeBiYPelsPerMeter(FILE* file) {
	int print_resolution = 0;
	fwrite(&print_resolution, 4, 1, file);
}

void writeBiClrUsed(FILE* file) {
	int colors_in_color_index = 0;
	fwrite(&colors_in_color_index, 4, 1, file);
}

void writeBiClrImportant(FILE* file) {
	int count_of_important_colors = 0;
	fwrite(&count_of_important_colors, 4, 1, file);
}

void writeHeader(struct Bitmap bitmap, FILE* file) {
	/* Writing Bitmap File Header (14 bytes long) */
	writeBfType(file);
	writeBfSize(file, bitmap.width, bitmap.height);
	writeBfReserved1(file);
	writeBfReserved2(file);
	writeBfOffBits(file);
	/* Writing Bitmap File Header (14 bytes long) */
	/* Writing Bitmap Info Header (40 bytes long) */
	writeBiSize(file);
	writeBiWidth(file, bitmap.width);
	writeBiHeight(file, bitmap.height);
	writeBiPlanes(file);
	writeBiBitCount(file);
	writeBiCompression(file);
	writeBiSizeImage(file, bitmap.width, bitmap.height);
	writeBiXPelsPerMeter(file);
	writeBiYPelsPerMeter(file);
	writeBiClrUsed(file);
	writeBiClrImportant(file);
	/* Writing Bitmap Info Header (40 bytes long) */
}

void writePixelData(struct Bitmap bitmap, FILE* file) {
	int row_size = calculateRowSize(bitmap.width);
	for(int y = bitmap.height - 1; y >= 0; y--) {
		for(int x = 0; x < bitmap.width; x++) {
			color_t color = bitmap.pixels[y * bitmap.width + x];
			// 0xBB0000 [B,G,R] <= 0x0000BB [R,G,B]
			unsigned char red = (color & 0xFF0000) >> 16;
			unsigned char green = (color & 0x00FF00) >> 8;
			unsigned char blue = (color & 0x0000FF);

			fwrite(&blue, 1, 1, file);
			fwrite(&green, 1, 1, file);
			fwrite(&red, 1, 1, file);
		}
		for(int i = bitmap.width * 3; i < row_size; i++) {
			fputc(0, file);
		}
	}
}

void writeBitmapFile(struct Bitmap bitmap, char fileName[]) {
	FILE* file = fopen(fileName, "wb");
	if(file == NULL) {
		return;
	}
	writeHeader(bitmap, file);
	writePixelData(bitmap, file);
	fclose(file);
}

void fill(struct Bitmap bitmap, int x1, int x2, int y1, int y2, color_t color) {
	for(int y = y1; y <= y2; y++) {
		for(int x = x1; x <= x2; x++) {
			setPixel(bitmap, x, y, color);
		}
	}
}

int main(int argc, char** argv) {
	struct Bitmap bitmap = init_bitmap(12, 12);
	char fileName[9] = "cool.bmp";
	fill(bitmap, 0, 12, 0, 4, 0xFF9933);
	fill(bitmap, 0, 12, 4, 8, 0xFFFFFF);
	fill(bitmap, 0, 12, 8, 12, 0x00FF00);
	writeBitmapFile(bitmap, fileName);
	free_bitmap(bitmap);
	return 0;
}
