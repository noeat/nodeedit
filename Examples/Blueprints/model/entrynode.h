#pragma once
#include "basenodeproxy.h"

class entrynode : public basenodeproxy
{
public:
	~entrynode() override {}
	void initlize(rapidjson::Value& config) override;
	entrynode* clone() override {};
	void onRegister() override {};
	void onRemove() override {};
private:

};