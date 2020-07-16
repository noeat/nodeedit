#include "addpin.h"
#include "model/settingproxy.h"
#include "define.h"
#include "view/leftpanemediator.h"
#include "model/mainboardproxy.h"
#include "view/workspacemediator.h"
#include "model/language.h"

void addpin::execute(PureMVC::INotification* note)
{
	PureMVC::IFacade *facade = this->getFacade();
	int nodeid = reinterpret_cast<int>(note->getBody());
	mainboardproxy* proxy = dynamic_cast<mainboardproxy*>(
		facade->retrieveProxy(mainboardproxy::NAME));
	assert(proxy);
	Node* node = proxy->getnode(nodeid);
	assert(node && (node->type == NODETYPE::SEQUENCE || node->type == NODETYPE::PARALLEL));
	
	languageproxy* language = dynamic_cast<languageproxy*>(
		facade->retrieveProxy(languageproxy::NAME));
	assert(language != nullptr);

	Pin pin;
	pin.id = proxy->next();
	pin.node = node;
	pin.type = PinType::Flow;
	pin.kind = PinKind::Output;
	snprintf(pin.comment, sizeof(pin.comment), language->getstr("branch"), node->outputs.size() + 1);
	node->outputs.push_back(pin);
	proxy->registerPin(node);
}