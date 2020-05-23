#pragma once
#include "pmvcpp.h"
#include <string>
#include <map>

class languageproxy : public PureMVC::Proxy
{
public:
	languageproxy();
	void onRegister();
	void onRemove() {};
	const char* getstr(const std::string& key) const 
	{ 
		return languages_.find(key) == languages_.end() ? nullptr 
			: languages_.at(key).c_str();
	}

	const static std::string NAME;
private:
	std::map<std::string, std::string> languages_;
	
};