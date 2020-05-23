#pragma once
#include "define.h"
#include <string>
#include <string.h>

inline void set_ui_str(ui_string& dest, const char* src)
{
	strncpy_s(dest, src, UISTR_LEN);
}

inline void set_ui_str(ui_string& dest, const std::string& src)
{
	set_ui_str(dest, src.c_str());
}