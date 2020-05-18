#include "settingproxy.h"
#include <iostream>
#include <rapidjson/document.h>
#include <fstream>
#include "utiliti.h"

void settingproxy::onRegister()
{
	const char* filepath = "setting.json";
	std::ifstream iff(filepath);
	if (!iff.good())
	{
		FatalError("Cant Open setting.json");
		return;
	}

	std::string filebuff((std::istreambuf_iterator<char>(iff)), std::istreambuf_iterator<char>());
	rapidjson::Document doc;
	if (doc.Parse(filebuff).HasParseError())
	{
		FatalError("Parse setting.json Failed");
		return;
	}

	if (doc.HasMember("workspacedir") && doc["workspacedir"].IsString())
	{
		this->workspace_ = doc["workspacedir"].GetString();
	}

	if (!doc.HasMember("confignode") || !doc["confignode"].IsArray())
	{
		FatalError("Parse setting.json Failed, confignode not exists");
	}
	
	auto& confignode = doc["confignode"];
	for (auto it = confignode.Begin(); it != confignode.End(); ++it)
	{
		
	}

	this->setData(this);
}

