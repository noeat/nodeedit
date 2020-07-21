#pragma once
#include "pmvcpp.h"
#include <string>
#include <map>
#include "define.h"

class basenodeproxy;
class settingproxy : public PureMVC::Proxy
{
public:
	settingproxy(std::string proxyName) : Proxy(proxyName) {}
	void onRegister();
	void onRemove();

	void saveworkspace(const char* str, const char* notepad);
	const std::string& workspace() const { return workspace_; }
	const std::string& notepad() const { return notepad_; }
	static std::string NAME;

	const std::vector<ConfNode*>& mainmenu() const { return menus_; }
private:
	std::string workspace_;
	std::string notepad_;
	std::vector<ConfNode*> menus_;
};