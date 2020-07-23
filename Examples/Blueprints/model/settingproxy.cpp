
#include "utiliti.h"
#include "settingproxy.h"
#include <fstream>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/ostreamwrapper.h>
#include <model/entrynode.h>
#include <algorithm>

std::string settingproxy::NAME = "settingproxy";
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

	if (doc.HasMember("notepad") && doc["notepad"].IsString())
	{
		this->notepad_ = doc["notepad"].GetString();
	}

	if (!doc.HasMember("confignode") || !doc["confignode"].IsArray())
	{
		FatalError("Parse setting.json Failed, confignode not exists");
	}

	auto& confignode = doc["confignode"];
	for (auto it = confignode.Begin(); it != confignode.End(); ++it)
	{
		const auto& value = *it;
		ConfNode* node = new ConfNode;
		this->menus_.push_back(node);
		node->name = value["name"].GetString();
		node->type = value["type"].GetInt();
		node->comment = value["comment"].GetString();
		if (value.HasMember("inputs"))
		{
			for (auto iter = value["inputs"].Begin(); iter != value["inputs"].End(); ++iter)
			{
				ConfPin pin;
				pin.name = (*iter)["name"].GetString();
				pin.type = (*iter)["type"].GetInt();
				pin.comment = (*iter)["comment"].GetString();
				node->inputs.emplace_back(pin);
			}
		}

		if (value.HasMember("outputs"))
		{
			for (auto iter = value["outputs"].Begin(); iter != value["outputs"].End(); ++iter)
			{
				ConfPin pin;
				pin.name = (*iter)["name"].GetString();
				pin.type = (*iter)["type"].GetInt();
				pin.comment = (*iter)["comment"].GetString();
				node->outputs.emplace_back(pin);
			}
		}
	}

	std::sort(this->menus_.begin(), this->menus_.end(),
		[](const ConfNode* left, const ConfNode* right)
	{
		return left->type < right->type;
	});
}

void settingproxy::onRemove()
{
	for (auto p : this->menus_)
	{
		delete p;
	}

	this->menus_.clear();
}

void settingproxy::saveworkspace(const char* str, const char* notepad)
{
	if (this->workspace_ != std::string(str) || this->notepad_ != std::string(notepad))
	{
		this->workspace_ = str;
		this->notepad_ = notepad;
		const char* filepath = "setting.json";
		std::ifstream iff(filepath);
		if (!iff.good())
		{
			FatalError("Cant Open setting.json");
			return;
		}

		std::string filebuff((std::istreambuf_iterator<char>(iff)), std::istreambuf_iterator<char>());
		iff.close();

		rapidjson::Document doc;
		if (doc.Parse(filebuff).HasParseError())
		{
			FatalError("Parse setting.json Failed");
			return;
		}
		auto& value = doc["workspacedir"];
		value.SetString(str, strlen(str));

		auto& jsnotepad = doc["notepad"];
		jsnotepad.SetString(notepad, strlen(notepad));
		
		std::ofstream ofs(filepath);
		rapidjson::OStreamWrapper osw(ofs);

		rapidjson::PrettyWriter<rapidjson::OStreamWrapper> writer(osw);
		doc.Accept(writer);
	}
	
}