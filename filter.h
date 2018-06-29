#include <source.h>
#include <sink.h>
#include <image.h>

class Filter : public Source, public Sink 
{ 
public:
	virtual const char * FilterName()=0;
	virtual const char * SourceName();
	virtual const char * SinkName();

	virtual void Update(); 
};

class Shrinker : public Filter { 
public: 
	virtual const char * FilterName();
	virtual void Execute(); 
};

class LRCombine : public Filter {
public: 
	virtual const char * FilterName();
	virtual void Execute(); 
};

class TBCombine : public Filter {
public:
	virtual const char * FilterName();
	virtual void Execute(); 
};

class Blender : public Filter 
{
private:
	double factor;

public:
	void SetFactor(double fact) { factor = fact; }
	virtual const char * FilterName();
	virtual void Execute(); 
};

class Mirror : public Filter
{
public:
	virtual const char * FilterName();
	virtual void Execute();
};

class Rotate : public Filter
{
public:
	virtual const char * FilterName();
	virtual void Execute();
};

class Subtract : public Filter
{
public:
	virtual const char * FilterName();
	virtual void Execute();
};

class Grayscale : public Filter
{
public:
	virtual const char * FilterName();
	virtual void Execute();
};

class Blur : public Filter
{
public:
	virtual const char * FilterName();
	virtual void Execute();
};