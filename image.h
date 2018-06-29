#ifndef IMAGE_H
#define IMAGE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Pixel
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
} Pixel;

class Source;

class Image
{
private:
	int height;
	int width;
	Pixel *data;
	Source *src;

public:
	Image(void);											//default constructor
	Image(int width, int height, Pixel *);					//parameterized contructor
	Image(int width, int height);							//parameterized constructor			
	Image(Image &);											//copy constructor
	~Image(void);											//destructor

	int GetWidth() const { return width; };
	int GetHeight() const { return height; };
	Pixel * GetData() const { return data; };
	
	void SetData(Pixel *input);
	void SetSource(Source *s);
	
	void ResetSize(int width, int height);					//reset width, height of image
	void Update() const;
};

#endif