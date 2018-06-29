#include <sink.h>

class PNMwriter : public Sink 
{
public:
	void Write(const char *);
	virtual const char * SinkName();
};