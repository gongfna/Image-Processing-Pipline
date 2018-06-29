#include <sink.h>
#include <stdio.h>

Sink::Sink() 			//initiate input values to NULL
{
	input1 = NULL;
	input2 = NULL;
}

void CheckSum::OutputCheckSum(const char *filename)
{
	int i, j, w1, h1;
	unsigned char r = 0;
	unsigned char g = 0;
	unsigned char b = 0;

	if (input1 == NULL) {
		char msg[1024];
		sprintf(msg, "Invalid number of inputs.");
		DataFlowException e(SinkName(), msg);
		throw e;
	}

	w1 = input1->GetWidth();
	h1 = input1->GetHeight();
	Pixel *data1 = input1->GetData(); 

	for(i=0; i < h1; i++) {
		for(j=0; j < w1; j++) {
			r += data1[i * w1 + j].r;
			g += data1[i * w1 + j].g;
			b += data1[i * w1 + j].b;
		}
	}

	FILE *f = fopen(filename, "w");
	fprintf(f, "CHECKSUM: %d, %d, %d\n", r, g, b);
	return;
}

const char *CheckSum::SinkName() { return "CheckSum"; }