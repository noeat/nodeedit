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

inline std::string id2mediatorname(int id)
{
	return std::string("mediator_") + std::to_string(id);
}