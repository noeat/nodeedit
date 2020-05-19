#include "basenodeproxy.h"
#include <assert.h>
#include "common.h"
#include "utiliti.h"

void basenodeproxy::initlize(rapidjson::Value& config)
{	
	if (config.HasMember("name") && config["name"].IsString())
	{
		set_ui_str(this->name_, config["name"].GetString());
	}
	else
	{
		FatalError("parse setting json node failed, cant has name field");
	}

	if (config.HasMember("type") && config["type"].IsInt())
	{
		this->type_ = config["type"].GetInt();
	}
	else
	{
		FatalError("parse node[%s] json failed, cant has type field", this->name_);
	}

	if (config.HasMember("comment") && config["name"].IsString())
	{
		set_ui_str(this->comment_, config["comment"].GetString());
	}
	else
	{
		FatalError("parse node[%s] json failed, cant has comment field", this->name_);
	}
}