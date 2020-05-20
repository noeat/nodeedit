#include "mainboardproxy.h"

const std::string mainboardproxy::NAME = "mainboardproxy";
mainboardproxy::mainboardproxy()
	:PureMVC::Proxy(mainboardproxy::NAME)
{

}

void mainboardproxy::onRegister()
{
	this->showstyleeditor_ = false;
	this->setData(this);
}

void mainboardproxy::onRemove()
{

}