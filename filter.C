#include <filter.h>
#include <logging.h>

void Filter::Update()			 						//makes sure inputs are up to date
{
	
	char msg[128];

	if (input1 != NULL) { 
		sprintf(msg, "%s: about to update input1", SourceName());
		Logger::LogEvent(msg);
		input1->Update();
		sprintf(msg, "%s: done updating input1", SourceName());
		Logger::LogEvent(msg);
	}

	if (input2 != NULL) { 
		sprintf(msg, "%s: about to update input2", SourceName());
		Logger::LogEvent(msg);
		input2->Update();
		sprintf(msg, "%s: done updating input2", SourceName());
		Logger::LogEvent(msg);
	}

	sprintf(msg, "%s: about to execute", SourceName());
	Logger::LogEvent(msg);
	Execute();											//once inputs are up to date, execute
	sprintf(msg, "%s: done executing", SourceName());
	Logger::LogEvent(msg);
}

const char * Filter::SourceName() { return FilterName(); }

const char * Filter::SinkName() { return FilterName(); }

const char * Shrinker::FilterName() { return "Shrinker"; }

const char * LRCombine::FilterName() { return "LRCombine"; }

const char * TBCombine::FilterName() { return "TBCombine"; }

const char * Blender::FilterName() { return "Blender"; }

const char * Mirror::FilterName() { return "Mirror"; }

const char * Rotate::FilterName() { return "Rotate"; }

const char * Subtract::FilterName() { return "Subtract"; }

const char * Grayscale::FilterName() { return "Grayscale"; }

const char * Blur::FilterName() { return "Blur"; }


void Shrinker::Execute() 
{
	int i, j, w1, h1, wOut, hOut;

	if (input1 == NULL || input2 != NULL)
	{
		char msg[1024];
		sprintf(msg, "Invalid number of inputs for Shrinker.");
		DataFlowException e(SinkName(), msg);
		throw e;
	}

	w1 = input1->GetWidth();
	h1 = input1->GetHeight();
	output.ResetSize(w1 / 2, h1 / 2);
	wOut = output.GetWidth();
	hOut = output.GetHeight();
	Pixel *data1 = input1->GetData();
	Pixel *data = output.GetData();

	for(i=0; i < hOut; i++) {
		for(j=0; j < wOut; j++) {
			data[i * wOut + j] = data1[2*i * w1 + 2*j];
		}
	}
	return;
}

void LRCombine::Execute()
{
	int i, j, wOut, hOut, w1, w2, h1, h2;

	if (input1 == NULL || input2 == NULL)
	{
		char msg[1024];
		sprintf(msg, "Invalid number of inputs for LRCombine.");
		DataFlowException e(SinkName(), msg);
		throw e;
	}

	h1 = input1->GetHeight();
	h2 = input2->GetHeight();
	w1 = input1->GetWidth();
	w2 = input2->GetWidth();

	if(h1 != h2)
	{
		char msg[1024];
		sprintf(msg, "heights must match: %d %d", h1, h2);
		DataFlowException e(SinkName(), msg);
		throw e;
	}

	output.ResetSize(w1 + w2, h1);
	wOut = output.GetWidth();
	hOut = output.GetHeight();
	Pixel *data1 = input1->GetData();
	Pixel *data2 = input2->GetData();
	Pixel *data = output.GetData();

	for(i=0; i < hOut; i++) {
		for(j=0; j < wOut; j++) {
			if(j < w1) {
				data[i * wOut + j] = data1[i * w1 + j];
			} else {
				data[i * wOut + j] = data2[i * w2 + (j - w1)];
			}
		}
	}
	return;
}


void TBCombine::Execute()
{
	int i, j, wOut, hOut, w1, h1, w2, h2;

	if (input1 == NULL || input2 == NULL)
	{
		char msg[1024];
		sprintf(msg, "Invalid number of inputs for TBCombine.");
		DataFlowException e(SinkName(), msg);
		throw e;
	}

	w1 = input1->GetWidth();
	h1 = input1->GetHeight();
	w2 = input2->GetWidth();
	h2 = input2->GetHeight();

	if(w1 != w2)
	{
		char msg[1024];
		sprintf(msg, "widths must match: %d %d", w1, w2);
		DataFlowException e(SinkName(), msg);
		throw e;
	}

	output.ResetSize(w1, h1 + h2);
	wOut = output.GetWidth();
	hOut = output.GetHeight();
	Pixel *data1 = input1->GetData();
	Pixel *data2 = input2->GetData();
	Pixel *data = output.GetData();

	for(i=0; i < hOut; i++) {
		for(j=0; j < wOut; j++) {
			if(i < h1) {
				data[i * wOut + j] = data1[i * w1 + j];
			} else {
				data[i * wOut + j] = data2[(i - h1) * w2 + j];
			}
		}
	}
	return;
}


void Blender::Execute()
{
	int i, j, wOut, hOut, w1, w2, h1, h2;

	if (input1 == NULL || input2 == NULL)
	{
		char msg[1024];
		sprintf(msg, "Invalid number of inputs for Blender.");
		DataFlowException e(SinkName(), msg);
		throw e;
	}

	w1 = input1->GetWidth();
	w2 = input2->GetWidth();
	h1 = input1->GetHeight();
	h2 = input2->GetHeight();

	if(w1 != w2 || h1 != h2)
	{
		char msg[1024];
		sprintf(msg, "widths and heights must match: %d %d, %d %d", w1, w2, h1, h2);
		DataFlowException e(SinkName(), msg);
		throw e;
	}

	if(factor > 1 || factor <= 0)
	{
		char msg[1024];
		sprintf(msg, "factor must be less than 1 and greater than 0: %f", factor);
		DataFlowException e(SinkName(), msg);
		throw e;
	}

	output.ResetSize(w1, h1);
	Pixel *data1 = input1->GetData();
	Pixel *data2 = input2->GetData();
	Pixel *data = output.GetData();

	for(i=0; i < h1; i++) {
		for(j=0; j < w1; j++) {
			data[i * w1 + j].r = (data1[i * w1 + j].r * factor) + (data2[i * w2 + j].r * (1 - factor));
			data[i * w1 + j].g = (data1[i * w1 + j].g * factor) + (data2[i * w2 + j].g * (1 - factor));
			data[i * w1 + j].b = (data1[i * w1 + j].b * factor) + (data2[i * w2 + j].b * (1 - factor));
		}
	}

	return;
}

void Mirror::Execute()
{
	int i, j, w1, h1;

	if (input1 == NULL || input2 != NULL)
	{
		char msg[1024];
		sprintf(msg, "Invalid number of inputs for Mirror.");
		DataFlowException e(SinkName(), msg);
		throw e;
	}

	w1 = input1->GetWidth();
	h1 = input1->GetHeight();
	output.ResetSize(w1, h1);
	Pixel *data1 = input1->GetData();
	Pixel *data = output.GetData();

	for(i=0; i < h1; i++) {
		for(j=0; j < w1; j++) {
			data[i * w1 + j] = data1[i * w1 + (w1 - j)];
		}
	}
	return;
}

void Rotate::Execute()
{
	int i, j, w1, h1;

	if (input1 == NULL || input2 != NULL)
	{
		char msg[1024];
		sprintf(msg, "Invalid number of inputs Rotate.");
		DataFlowException e(SinkName(), msg);
		throw e;
	}

	w1 = input1->GetWidth();
	h1 = input1->GetHeight();
	output.ResetSize(h1, w1);
	Pixel *data1 = input1->GetData();
	Pixel *data = output.GetData();

	for(i=0; i < w1; i++) {
		for(j=0; j < h1; j++) {
			data[i * h1 + j] = data1[(h1 - 1 - j) * w1 + i];
		}
	}
	return;
}

void Subtract::Execute()
{
	int i, j , w1, h1, w2, h2, index;

	if (input1 == NULL || input2 == NULL)
	{
		char msg[1024];
		sprintf(msg, "Invalid number of inputs for Subtract.");
		DataFlowException e(SinkName(), msg);
		throw e;
	}

	w1 = input1->GetWidth();
	w2 = input2->GetWidth();
	h1 = input1->GetHeight();
	h2 = input2->GetHeight();

	if(w1 != w2 || h1 != h2)
	{
		char msg[1024];
		sprintf(msg, "widths and heights must match: %d %d, %d %d", w1, w2, h1, h2);
		DataFlowException e(SinkName(), msg);
		throw e;
	}

	output.ResetSize(w1, h1);
	Pixel *data1 = input1->GetData();
	Pixel *data2 = input2->GetData();
	Pixel *data = output.GetData();

	for(i=0; i < h1; i++) {
		for(j=0; j < w1; j++) {
			index = i * w1 + j;

			if(data1[index].r > data2[index].r) {
				data[index].r = data1[index].r - data2[index].r;
			} else {
				data[index].r = 0;
			}

			if(data1[index].g > data2[index].g) {
				data[index].g = data1[index].g - data2[index].g;
			} else {
				data[index].g = 0;
			}

			if(data1[index].b > data2[index].b) {
				data[index].b = data1[index].b - data2[index].b;
			} else {
				data[index].b = 0;
			}
		}
	}
	return;
}

void Grayscale::Execute()
{
	int i, j, w1, h1;

	if (input1 == NULL || input2 != NULL)
	{
		char msg[1024];
		sprintf(msg, "Invalid number of inputs for Grayscale.");
		DataFlowException e(SinkName(), msg);
		throw e;
	}

	w1 = input1->GetWidth();
	h1 = input1->GetHeight();
	output.ResetSize(w1, h1);
	Pixel *data1 = input1->GetData();
	Pixel *data = output.GetData();

	for(i=0; i < h1; i++) {
		for(j=0; j < w1; j++) {
			data[i * w1 + j].r = data1[i * w1 + j].r / 5 + data1[i * w1 + j].g / 2 + data1[i * w1 + j].b / 4;
			data[i * w1 + j].g = data1[i * w1 + j].r / 5 + data1[i * w1 + j].g / 2 + data1[i * w1 + j].b / 4;
			data[i * w1 + j].b = data1[i * w1 + j].r / 5 + data1[i * w1 + j].g / 2 + data1[i * w1 + j].b / 4;
		}
	}
	return;
}

void Blur::Execute()
{
	int i, j, w1, h1;

	if (input1 == NULL || input2 != NULL)
	{
		char msg[1024];
		sprintf(msg, "Invalid number of inputs for Blur.");
		DataFlowException e(SinkName(), msg);
		throw e;
	}

	w1 = input1->GetWidth();
	h1 = input1->GetHeight();
	output.ResetSize(w1, h1);
	Pixel *data1 = input1->GetData();
	Pixel *data = output.GetData();

	for(i=0; i < h1; i++) {
		for(j=0; j < w1; j++) {
			if(i == 0 || j == 0 || i == (h1 - 1) || j == (w1 - 1)) {
				data[i * w1 + j].r = data1[i * w1 + j].r;
				data[i * w1 + j].g = data1[i * w1 + j].g;
				data[i * w1 + j].b = data1[i * w1 + j].b;
			} else {
				data[i * w1 + j].r = data1[(i-1) * w1 + (j-1)].r / 8 + data1[i * w1 + (j-1)].r / 8
										+ data1[(i-1) * w1 + j].r / 8 + data1[(i+1) * w1 + j].r / 8
										+ data1[(i-1) * w1 + (j+1)].r / 8 + data1[i * w1 + (j+1)].r / 8
										+ data1[(i+1) * w1 + (j+1)].r / 8 + data1[(i+1) * w1 + (j-1)].r / 8;

				data[i * w1 + j].g = data1[(i-1) * w1 + (j-1)].g / 8 + data1[i * w1 + (j-1)].g / 8
										+ data1[(i-1) * w1 + j].g / 8 + data1[(i+1) * w1 + j].g / 8
										+ data1[(i-1) * w1 + (j+1)].g / 8 + data1[i * w1 + (j+1)].g / 8 
										+ data1[(i+1) * w1 + (j+1)].g / 8 + data1[(i+1) * w1 + (j-1)].g / 8;

				data[i * w1 + j].b = data1[(i-1) * w1 + (j-1)].b / 8 + data1[i * w1 + (j-1)].b / 8
										+ data1[(i-1) * w1 + j].b / 8 + data1[(i+1) * w1 + j].b / 8
										+ data1[(i-1) * w1 + (j+1)].b / 8 + data1[i * w1 + (j+1)].b / 8
										+ data1[(i+1) * w1 + (j+1)].b / 8 + data1[(i+1) * w1 + (j-1)].b / 8;
			}
		}
	}
	return;
}