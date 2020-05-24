#pragma once
#include "basenodeproxy.h"

class entrynode : public basenodeproxy
{
public:
	entrynode(int id);
	~entrynode() override {}
	void initlize(const ConfNode* node) override;
	entrynode* clone() override { return nullptr; };
	void onRegister() override {};
	void onRemove() override {};
private:
	ed::PinId pin_;

};