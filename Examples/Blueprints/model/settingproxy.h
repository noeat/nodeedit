#pragma once
#include "pmvcpp.h"
#include <string>
#include <map>

class settingproxy : public PureMVC::Proxy
{
public:
	settingproxy(std::string proxyName) : Proxy(proxyName) {}
	void onRegister();
	void onRemove();

	const std::string& workspace() const { return workspace_; }
	static std::string NAME;
private:
	std::string workspace_;
};