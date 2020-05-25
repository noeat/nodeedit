#include "mainmenuclick.h"
#include "model/settingproxy.h"
#include <assert.h>
#include "define.h"
#include "model/mainboardproxy.h"
#include "view/entrymediator.h"

void mainmenuclick::execute(PureMVC::INotification* note)
{
	PureMVC::IFacade *facade = this->getFacade();
	std::pair<ConfNode*, ImVec2*>* data = (std::pair<ConfNode*, ImVec2*>*)note->getBody();	
	mainboardproxy* proxy = dynamic_cast<mainboardproxy*>(
		facade->retrieveProxy(mainboardproxy::NAME));
	assert(proxy);
	Node* n = proxy->addnode(data->first, *data->second);
	PureMVC::Mediator* mediator = nullptr;
	switch (data->first->type)
	{
	case NODETYPE::ENTRY:
		mediator = new entrymediator(n->id.Get());
		break;
	default:
		break;
	}

	if (mediator != nullptr)
	{
		ed::SetNodePosition(n->id, n->position);
		facade->registerMediator(mediator);
	}
}