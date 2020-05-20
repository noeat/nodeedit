#pragma once
#include "pmvcpp.h"
#include <rapidjson/document.h>
#include "define.h"

class basenodeproxy : PureMVC::Proxy
{
public:
	~basenodeproxy() {}
	virtual basenodeproxy* clone() = 0;
	virtual void initlize(rapidjson::Value& config);

	ed::NodeId id() const { return ID_; }

	int type() const { return type_; }

	const ui_string& name() const { return name_; }
	ui_string& name() { return name_; }

	const ui_string& comment() const { return comment_; }
	ui_string& comment() { return comment_; }

protected:
	ed::NodeId ID_;
	int type_;
	ui_string name_;
	ui_string comment_;
};