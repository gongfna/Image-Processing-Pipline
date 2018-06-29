#include <source.h>


Source::Source()
{
	output.SetSource(this);
}

Image * Source::GetOutput() 
{ 
	return &output; 
}

void Source::Update()
{	
	char msg[128];
	sprintf(msg, "%s: about to execute", SourceName());
	Logger::LogEvent(msg);
	Execute();
	sprintf(msg, "%s: done executing", SourceName());
	Logger::LogEvent(msg);
}

Color::Color(int width, int height, int r, int g, int b)
{
	int i, j;

	if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255) {
		char msg[1024];
		sprintf(msg, "color channels must have values between 0 and 255: r: %d g: %d b: %d", r, g, b);
		DataFlowException e(SourceName(), msg);
		throw e;
	}

	output.ResetSize(width, height);
	Pixel *data = output.GetData();

	for(i=0; i < height; i++) {
		for(j=0; j < width; j++) {
			data[i * width + j].r = r;
			data[i * width + j].g = g;
			data[i * width + j].b = b;
		}
	}
}

const char * Color::SourceName()
{
	return "ConstantColor";
}