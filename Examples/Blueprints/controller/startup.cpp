#include "startup.h"
#include "model/settingproxy.h"

void startup::execute(PureMVC::INotification* note)
{
	PureMVC::IFacade *facade = this->getFacade();
	auto *setting = new settingproxy(settingproxy::NAME);
	facade->registerProxy(setting);

	if (setting->workspace() == "")
	{
		facade->re
	}
}