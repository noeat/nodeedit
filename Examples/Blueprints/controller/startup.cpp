#include "startup.h"
#include "model/settingproxy.h"
#include "define.h"
#include "view/leftpanemediator.h"
#include "model/mainboardproxy.h"

void startup::execute(PureMVC::INotification* note)
{
	PureMVC::IFacade *facade = this->getFacade();
	auto *setting = new settingproxy(settingproxy::NAME);
	facade->registerProxy(setting);
	facade->registerMediator(new leftpanemediator());
	facade->registerProxy(new mainboardproxy());
	if (setting->workspace() == "")
	{
		facade->sendNotification(COMMANDTYPE::SETTING);
	}
	else
	{

	}
}