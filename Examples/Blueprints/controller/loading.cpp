#include "loading.h"
#include "model/settingproxy.h"
#include "define.h"
#include "view/leftpanemediator.h"
#include "model/mainboardproxy.h"
#include "view/workspacemediator.h"
#include "model/language.h"
#include "view/mainmediator.h"
#include "view/mainmeunmediator.h"
#include "view/linkmediator.h"
#include "view/deletemediator.h"

void loading::execute(PureMVC::INotification* note)
{
	PureMVC::IFacade *facade = this->getFacade();
	facade->registerMediator(new leftpanemediator());
	facade->registerProxy(new mainboardproxy());
	facade->registerMediator(new mainmediator());
	facade->registerMediator(new mainmeunmediator());
	facade->registerMediator(new linkmediator());
	facade->registerMediator(new deletemediator());

	settingproxy* setting = dynamic_cast<settingproxy*>(
		facade->retrieveProxy(settingproxy::NAME));
	assert(setting != nullptr);
	setting->saveworkspace(((std::pair<const char*, const char*>*)note->getBody())->first,
		((std::pair<const char*, const char*>*)note->getBody())->second);
	loadworkspace(((std::pair<const char*, const char*>*)note->getBody())->first);
	facade->removeMediator(workspacemediator::NAME);
}

void loading::loadworkspace(const char* dir)
{

}