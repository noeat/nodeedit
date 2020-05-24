#include "mainmenuclick.h"
#include "model/settingproxy.h"
#include <assert.h>
#include "define.h"
#include "model/mainboardproxy.h"
#include "view/entrymediator.h"

void mainmenuclick::execute(PureMVC::INotification* note)
{
	PureMVC::IFacade *facade = this->getFacade();
	ConfNode* node = (ConfNode*)note->getBody();
	
	mainboardproxy* proxy = dynamic_cast<mainboardproxy*>(
		facade->retrieveProxy(mainboardproxy::NAME));
	assert(proxy);
	Node* n = proxy->addnode(node);
	PureMVC::Mediator* mediator = nullptr;
	switch (node->type)
	{
	case NODETYPE::ENTRY:
		mediator = new entrymediator(n->id.Get());
		break;
	default:
		break;
	}

	if (mediator != nullptr)
	{
		facade->registerMediator(mediator);
	}
}