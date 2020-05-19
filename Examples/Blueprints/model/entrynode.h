#pragma once
#include "basenodeproxy.h"

class entrynode : public basenodeproxy
{
public:
	void initlize(rapidjson::Value& config);
	entrynode* clone();
	void onRegister();
	void onRemove();
private:

};