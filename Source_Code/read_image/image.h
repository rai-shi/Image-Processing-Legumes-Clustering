#pragma once

#include <string.h>

// image.h
typedef struct image {
	int w;
	int h;
	int c;
	unsigned char* data;
} image;

image load_image(const char* filename);
image make_image(int w, int h, int c);
image make_empty_image(int w, int h, int c);
image RGBtoIntensity(image im);

image Intensity2RGB(image im);


