#pragma once
#include "pmvcpp.h"

class basenodeproxy : PureMVC::Proxy
{
public:
	basenodeproxy();
	virtual basenodeproxy* clone() = 0;

private:
	int type;
};