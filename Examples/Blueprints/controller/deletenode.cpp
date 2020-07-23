#include "deletenode.h"
#include "model/settingproxy.h"
#include <assert.h>
#include "define.h"
#include "model/mainboardproxy.h"
#include "utiliti.h"
#include "common.h"

void deletenode::execute(PureMVC::INotification* note)
{
	PureMVC::IFacade *facade = this->getFacade();
	Node* node = (Node*)note->getBody();
	mainboardproxy* proxy = dynamic_cast<mainboardproxy*>(
		facade->retrieveProxy(mainboardproxy::NAME));
	assert(proxy);
	auto mediator = facade->removeMediator(id2mediatorname(node->id.Get()));
	delete mediator;
	proxy->removenode(node);
}