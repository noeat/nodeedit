#include "mainmeun.h"
#include "model/settingproxy.h"
#include <assert.h>

void mainmeun::execute(PureMVC::INotification* note)
{
	PureMVC::IFacade *facade = this->getFacade();
	settingproxy* proxy = dynamic_cast<settingproxy*>(
		facade->retrieveProxy(settingproxy::NAME));
	assert(proxy != nullptr);

	facade->sendNotification(COMMANDTYPE::DISPLAYMAINMENU,
		(void*)&proxy->mainmenu());
}