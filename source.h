#ifndef SOURCE_H
#define SOURCE_H

#include <image.h>
#include <logging.h>

class Source 
{
protected:
	Image output;
	virtual void Execute()=0;

public:
	Source(void);								// sets output.src as the current filter when a Source is created
	Image * GetOutput(void);
	virtual const char * SourceName()=0;
	virtual void Update();						// calls current source's Execute function
};

class Color : public Source
{
public: 
	Color(int width, int height, int r, int g, int b);
	virtual const char * SourceName();
	virtual void Execute() {};
};

#endif