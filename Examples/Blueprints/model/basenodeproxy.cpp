#include "basenodeproxy.h"
#include <assert.h>
#include "common.h"
#include "utiliti.h"

basenodeproxy::basenodeproxy(int id) :
	ID_(id)
{

}

void basenodeproxy::initlize(const ConfNode* node)
{	
	set_ui_str(this->name_, node->name);
	this->type_ = node->type;
	set_ui_str(this->comment_, node->comment);
}