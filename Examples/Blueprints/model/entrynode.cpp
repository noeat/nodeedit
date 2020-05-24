#include "entrynode.h"
#include <assert.h>
#include "common.h"
#include "utiliti.h"

entrynode::entrynode(int id)
	:basenodeproxy(id)
{

}

void entrynode::initlize(const ConfNode* node)
{
	basenodeproxy::initlize(node);
	assert(this->type_ == NODETYPE::ENTRY);
}