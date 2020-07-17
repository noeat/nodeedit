#include "deletemediator.h"
#include "define.h"
#include "imgui_node_editor.h"
#include "model/language.h"
#include "model/mainboardproxy.h"
#include "Application.h"
#include <ax/Builders.h>
#include <ax/Math2D.h>

const std::string deletemediator::NAME = "deletemediator";

namespace util = ax::NodeEditor::Utilities;



deletemediator::deletemediator()
	:PureMVC::Mediator(deletemediator::NAME)
{

}

std::vector<int> deletemediator::listNotificationInterests()
{
	return std::vector<int>{COMMANDTYPE::DISPLAYLINK};
}

void deletemediator::handleNotification(PureMVC::INotification* notification)
{
	PureMVC::IFacade *facade = this->getFacade();
	
	mainboardproxy* proxy = dynamic_cast<mainboardproxy*>(
		facade->retrieveProxy(mainboardproxy::NAME));
	assert(proxy);

	
	if (ed::BeginDelete())
	{
		ed::LinkId linkId = 0;
		while (ed::QueryDeletedLink(&linkId))
		{
			if (ed::AcceptDeletedItem())
			{
				auto pin = proxy->GetPin(linkId.Get());
				if (pin)
				{
					assert(pin->links.size() == 1);
					pin->links[0]->links.erase(std::remove(
						pin->links[0]->links.begin(),
						pin->links[0]->links.end(),
						pin), pin->links[0]->links.end());
					pin->links.clear();
				}
			}
		}

		ed::NodeId nodeId = 0;
		while (ed::QueryDeletedNode(&nodeId))
		{
			if (ed::AcceptDeletedItem())
			{
				auto node = proxy->getnode(nodeId.Get());
				if (node)
				{
					proxy->removenode(node);
				}				
			}
		}
	}
	ed::EndDelete();
}


void deletemediator::onRegister()
{
	
}

void deletemediator::onRemove()
{
	
}