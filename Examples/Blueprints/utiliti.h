#pragma once
#include <Windows.h>
void FatalError(const char* format, ...)
{
	char buff[1024];
	va_list list;
	va_start(list, format);
	vsnprintf(buff, 1024, format, list);
	va_end(list);		
	MessageBoxA(NULL, buff, "Error", MB_OK);
	exit(-1);
}
