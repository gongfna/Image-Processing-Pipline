#include <source.h>

class PNMreader : public Source 
{
protected:
	char *filename;

public:
	PNMreader(const char *);				 							//constructor
	~PNMreader() { if(filename != NULL) { free(filename); } }	//destructor

	virtual const char * SourceName();
	virtual void Execute();
};