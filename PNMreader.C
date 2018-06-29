#include <image.h>
#include <string.h>
#include <PNMreader.h>

PNMreader::PNMreader(const char *f) 
{
	filename = (char *) malloc(strlen(f) + 1); 	//new char[strlen(f) + 1];	
	strcpy(filename, f);											
} 

const char * PNMreader::SourceName() 
{
	return "PNMreader";
}

void PNMreader::Execute()
{
	char magicNum[128];
	int width, height, maxval;
	FILE *f_in = fopen(this->filename, "rb");

	fscanf(f_in, "%s\n%d %d\n%d\n", magicNum, &width, &height, &maxval);

	if (magicNum[0] != 'P' || magicNum[1] != '6')
	{
		char msg[1024];
		sprintf(msg, "Invalid file type. Must be P6 format.");
		DataFlowException e(SourceName(), msg);
		throw e;
	}

	if (maxval < 0 || maxval > 255)
	{
		char msg[1024];
		sprintf(msg, "Invalid file. Value of color channels must be between 0 and 255.");
		DataFlowException e(SourceName(), msg);
		throw e;
	}

	output.ResetSize(width, height);
	Pixel *out = output.GetData();
	fread(out, sizeof(Pixel), width * height, f_in);						//reading pixel data
	
	fclose(f_in);
	return;
}
