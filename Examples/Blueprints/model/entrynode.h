#pragma once
#include "basenodeproxy.h"

class entrynode : public basenodeproxy
{
public:
	entrynode() {};
	~entrynode() override {}
	void initlize(rapidjson::Value& config) override;
	entrynode* clone() override { return nullptr; };
	void onRegister() override {};
	void onRemove() override {};
private:
	ed::PinId flow;
	
};