#include "mainmenuclick.h"
#include "model/settingproxy.h"
#include <assert.h>
#include "define.h"
#include "model/mainboardproxy.h"
#include "view/entrymediator.h"
#include "view/seqeuemediator.h"
#include "view/conditionmediator.h"
#include "view/listenmediator.h"
#include "view/countermediator.h"
#include "view/waitmediator.h"
#include "view/eventwaitmediator.h"
#include "view/eventadaptermediator.h"
#include "view/actionmediator.h"
void mainmenuclick::execute(PureMVC::INotification* note)
{
	PureMVC::IFacade *facade = this->getFacade();
	std::pair<ConfNode*, ImVec2*>* data = (std::pair<ConfNode*, ImVec2*>*)note->getBody();	
	mainboardproxy* proxy = dynamic_cast<mainboardproxy*>(
		facade->retrieveProxy(mainboardproxy::NAME));
	assert(proxy);
	Node* n = proxy->addnode(data->first, *data->second);
	PureMVC::Mediator* mediator = nullptr;
	int type = data->first->type < 300 ? data->first->type : 300;
	switch (type)
	{
	case NODETYPE::ENTRY:
		mediator = new entrymediator(n->id.Get());
		break;
	case NODETYPE::SEQUENCE:
	case NODETYPE::PARALLEL:
		mediator = new seqeuemediator(n->id.Get());
		break;
	case NODETYPE::BRANCH:
		mediator = new conditionmediator(n->id.Get());
		break;
	case NODETYPE::LISTEN:
		mediator = new listenmediator(n->id.Get());
		break;
	case NODETYPE::REPEATED:
		mediator = new countermediator(n->id.Get());
		break;
	case NODETYPE::WAIT:
		mediator = new waitmediator(n->id.Get());
		break;
	case NODETYPE::EVENTWAIT:
		mediator = new eventwaitmediator(n->id.Get());
		break;
	case NODETYPE::EVENTADPTOR:
		mediator = new eventadaptermediator(n->id.Get());
		break;
	case NODETYPE::ACTION:
		mediator = new actionmediator(n->id.Get());
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