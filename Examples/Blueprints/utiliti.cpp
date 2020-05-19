#include "utiliti.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static FILE* createlog()
{
	const char* filename = "output.log";
	FILE* pf = fopen(filename, "a+");
	if (pf == NULL)
	{		
		exit(-1);
	}

	atexit(Flush);
	fseek(pf, 0, SEEK_END);
	return pf;
}

static FILE* pf = createlog();
static void log(const char* str)
{
	time_t rawtime;
	struct tm *info;
	char buffer[80];
	time(&rawtime);
	info = localtime(&rawtime);
	strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", info);
	char buff[1200];
	snprintf(buff, 1200, "[%s] %s\n", buffer, str);
	fputs(buff, pf);
}

void Flush()
{
	fflush(pf);
}

void FatalError(const char* format, ...)
{
	char buff[1024];
	va_list list;
	va_start(list, format);
	vsnprintf(buff, 1024, format, list);
	va_end(list);
	log(buff);
	Flush();
	exit(-1);
}

void LogMessage(const char* format, ...)
{
	char buff[1024];
	va_list list;
	va_start(list, format);
	vsnprintf(buff, 1024, format, list);
	va_end(list);
	log(buff);
}