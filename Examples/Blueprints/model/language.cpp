
#include "utiliti.h"
#include "language.h"
#include <fstream>
#include <rapidjson/document.h>

const std::string languageproxy::NAME = "languageproxy";
languageproxy::languageproxy()
	:PureMVC::Proxy(languageproxy::NAME)
{

}

void languageproxy::onRegister()
{
	const char* filepath = "language.json";
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
	
	for (auto iter = doc.MemberBegin(); iter != doc.MemberEnd(); ++iter)
	{
		this->languages_.insert(std::make_pair(iter->name.GetString(),
			iter->value.GetString()));
	}
}