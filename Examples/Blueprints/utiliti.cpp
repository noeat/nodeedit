#include "utiliti.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string>

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


tokenizer::tokenizer(const std::string &src, const char sep, int vectorReserve /*= 0*/, bool keepEmptyStrings /*= true*/)
{
	m_str = new char[src.length() + 1];
	memcpy(m_str, src.c_str(), src.length() + 1);

	if (vectorReserve)
		m_storage.reserve(vectorReserve);

	char* posold = m_str;
	char* posnew = m_str;

	for (;;)
	{
		if (*posnew == sep)
		{
			if (keepEmptyStrings || posold != posnew)
				m_storage.push_back(posold);

			posold = posnew + 1;
			*posnew = '\0';
		}
		else if (*posnew == '\0')
		{	
			if (posold != posnew)
				m_storage.push_back(posold);

			break;
		}

		++posnew;
	}
}

int GenID()
{
	static int s_id = 0;
	return ++s_id;
}