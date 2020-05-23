#include "loading.h"
#include "model/settingproxy.h"
#include "define.h"
#include "view/leftpanemediator.h"
#include "model/mainboardproxy.h"
#include "view/workspacemediator.h"
#include "model/language.h"
#include "view/mainmediator.h"
#include "view/mainmeunmediator.h"

void loading::execute(PureMVC::INotification* note)
{
	PureMVC::IFacade *facade = this->getFacade();
	facade->registerMediator(new leftpanemediator());
	facade->registerProxy(new mainboardproxy());
	facade->registerMediator(new mainmediator());
	facade->registerMediator(new mainmeunmediator());
	
	settingproxy* setting = dynamic_cast<settingproxy*>(
		facade->retrieveProxy(settingproxy::NAME));
	assert(setting != nullptr);
	setting->saveworkspace((char*)note->getBody());
	loadworkspace((char*)note->getBody());
	facade->removeMediator(workspacemediator::NAME);
}

void loading::loadworkspace(const char* dir)
{

}