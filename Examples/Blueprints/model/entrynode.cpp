#include "entrynode.h"
#include <assert.h>
#include "common.h"
#include "utiliti.h"

void entrynode::initlize(rapidjson::Value& config)
{
	basenodeproxy::initlize(config);
	assert(this->type_ == NODETYPE::ENTRY);

}