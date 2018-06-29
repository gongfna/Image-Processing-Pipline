#include <logging.h>
#include <stdio.h>

FILE* Logger::logger = fopen("logger.txt", "w");

DataFlowException::DataFlowException(const char *type, const char *error)
{
	sprintf(msg, "Throwing exception: (%s): %s", type, error);
	Logger::LogEvent(msg);
}

void Logger::LogEvent(const char *event)
{
	fprintf(logger, "%s\n", event);
}

void Logger::Finalize()
{
	fclose(logger);
}