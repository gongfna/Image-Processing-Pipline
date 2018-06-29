#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <image.h>
#include <source.h>

Image::Image(void)
{
	width = 0;
	height = 0;
	data = NULL;
}

Image::Image(int w, int h, Pixel* d)
{
	int k;
	width = w;
	height = h;
	data = (Pixel *) malloc(width * height * sizeof(Pixel));
	memcpy(data, d, sizeof(Pixel) * width * height);
}

Image::Image(int w, int h)
{
	width = w;
	height = h;
	data = (Pixel *) malloc(width * height * sizeof(Pixel));
}

Image::Image(Image &input)
{
	int k;
	width = input.width;
	height = input.height;
	data = (Pixel *) malloc(width * height * sizeof(Pixel));
	memcpy(data, &input, sizeof(Pixel) * width * height);
}

Image::~Image() 
{
	if (data != NULL) {
		free(data);
	}
}

void Image::ResetSize(int w, int h)
{
	width = w;
	height = h;
	data = (Pixel *) malloc(width * height * sizeof(Pixel));
} 

void Image::SetData(Pixel *input)
{
	if (data == NULL) 
	{
		data = (Pixel *) malloc(width * height * sizeof(Pixel));
	}
	memcpy(data, input, sizeof(Pixel) * width * height);
}

void Image::SetSource(Source *s)
{
	if (s != NULL) { src = s; }
}

void Image::Update() const
{ 
	if (src != NULL) { src->Update(); }
}

