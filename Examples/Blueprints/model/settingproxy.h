#pragma once
#include "pmvcpp.h"
#include <string>
#include <map>

class settingproxy : PureMVC::Proxy
{
public:
	settingproxy(std::string proxyName) : Proxy(proxyName) {}
	void onRegister();
	void onRemove();
private:
	std::string workspace_;
};