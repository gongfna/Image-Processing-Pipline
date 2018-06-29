#include <image.h>
#include <PNMwriter.h>

const char * PNMwriter::SinkName()
{
	return "PNMwriter";
}

void PNMwriter::Write(const char *filename) 
{
	if (input1 == NULL) {
		char msg[1024];
		sprintf(msg, "Invalid number of inputs.");
		DataFlowException e(SinkName(), msg);
		throw e;
	}

	FILE *f_out = fopen(filename, "wb");

	fprintf(f_out, "P6\n%d %d\n255\n", input1->GetWidth(), input1->GetHeight());				//pnm image format
	fwrite(input1->GetData(), sizeof(Pixel), input1->GetWidth() * input1->GetHeight(), f_out); 	//writing pixel data

	fclose(f_out);
	return;
}