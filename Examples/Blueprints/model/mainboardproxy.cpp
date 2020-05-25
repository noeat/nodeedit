#include "mainboardproxy.h"
#include "common.h"

const std::string mainboardproxy::NAME = "mainboardproxy";
mainboardproxy::mainboardproxy()
	:PureMVC::Proxy(mainboardproxy::NAME)
{

}

void mainboardproxy::onRegister()
{
	this->next_ = 0;
	this->showstyleeditor_ = false;
	this->setData(this);
}

void mainboardproxy::onRemove()
{

}

Node* mainboardproxy::addnode(const ConfNode* conf, const ImVec2& pos)
{
	Node* node = new Node();
	node->id = this->next();
	set_ui_str(node->name, conf->name);
	set_ui_str(node->comment, conf->comment);
	node->type = conf->type;
	node->show = true;
	node->position = pos;
	for (auto& item : conf->inputs)
	{
		Pin pin;
		pin.id = this->next();
		pin.node = node;
		set_ui_str(pin.name, item.name);
		set_ui_str(pin.comment, item.comment);
		pin.type = (PinType)item.type;
		pin.kind = PinKind::Input;
		if (pin.type == PinType::Int)
		{
			pin.value.int_ = 0;
		}
		else if (pin.type == PinType::Bool)
		{
			pin.value.int_ = 0;
		}
		else if (pin.type == PinType::Float)
		{
			pin.value.double_ = 0.0;
		}
		else if (pin.type == PinType::Object)
		{
			pin.value.str_[0] = 0;
		}
		else if (pin.type == PinType::String)
		{
			pin.value.str_[0] = 0;
		}

		node->inputs.push_back(pin);
	}

	for (auto& item : conf->outputs)
	{
		Pin pin;
		pin.id = this->next();
		pin.node = node;
		set_ui_str(pin.name, item.name);
		set_ui_str(pin.comment, item.comment);
		pin.type = (PinType)item.type;
		pin.kind = PinKind::Input;
		if (pin.type == PinType::Int)
		{
			pin.value.int_ = 0;
		}
		else if (pin.type == PinType::Bool)
		{
			pin.value.int_ = 0;
		}
		else if (pin.type == PinType::Float)
		{
			pin.value.double_ = 0.0;
		}
		else if (pin.type == PinType::Object)
		{
			pin.value.str_[0] = 0;
		}
		else if (pin.type == PinType::String)
		{
			pin.value.str_[0] = 0;
		}

		node->outputs.push_back(pin);
	}

	this->nodes_[node->id.Get()] = node;
	if (conf->type == NODETYPE::ENTRY)
	{
		this->skills_.push_back(node);
	}

	return node;
}