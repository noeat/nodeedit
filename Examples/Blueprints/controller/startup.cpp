#include "startup.h"
#include "model/settingproxy.h"
#include "define.h"
#include "view/leftpanemediator.h"
#include "model/mainboardproxy.h"
#include "view/workspacemediator.h"
#include "model/language.h"

void startup::execute(PureMVC::INotification* note)
{
	PureMVC::IFacade *facade = this->getFacade();
	auto *setting = new settingproxy(settingproxy::NAME);
	facade->registerProxy(setting);
	facade->registerProxy(new languageproxy());
	facade->registerMediator(new workspacemediator());
	facade->sendNotification(COMMANDTYPE::SETTING, (void*)setting->workspace().c_str());
}