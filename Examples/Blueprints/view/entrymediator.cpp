#include "entrymediator.h"
#include "define.h"
#include "imgui_node_editor.h"
#include "model/language.h"
#include "common.h"


entrymediator::entrymediator(int nodeid)
	:PureMVC::Mediator(id2mediatorname(nodeid)), nodeid_(nodeid)
{
	
}

std::vector<int> entrymediator::listNotificationInterests()
{
	return std::vector<int>{COMMANDTYPE::DISPLAYNODE + this->nodeid_};
}

void entrymediator::handleNotification(PureMVC::INotification* notification)
{
	Node* node = (Node*)notification->getBody();
	PureMVC::IFacade* facade = this->getFacade();
	assert(node != nullptr && node->type == NODETYPE::ENTRY);	
}


void entrymediator::onRegister()
{
	
}

void entrymediator::onRemove()
{
	
}