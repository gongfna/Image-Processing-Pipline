#ifndef SINK_H
#define SINK_H

#include <image.h>
#include <logging.h>

class Sink 
{
protected:
	const Image *input1;
	const Image *input2;

public:
	Sink(void);
	void SetInput(Image *input) { input1 = input; }
	void SetInput2(Image *input){ input2 = input; }
	virtual const char * SinkName()=0;
};

class CheckSum : public Sink 
{
public: 
	virtual const char * SinkName();
	void OutputCheckSum(const char *);

};

#endif