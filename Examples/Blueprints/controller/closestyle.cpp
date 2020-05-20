#include "closestyle.h"
#include "model/mainboardproxy.h"
#include "view/styleeditormediator.h"
#include <assert.h>
#include "define.h"

void closestylecmd::execute(PureMVC::INotification* note)
{
	PureMVC::IFacade* facade = this->getFacade();
	mainboardproxy* maindata = dynamic_cast<mainboardproxy*>(
		facade->retrieveProxy(mainboardproxy::NAME));
	assert(maindata != nullptr);
	maindata->showstyleeditor() = true;
	delete facade->removeMediator(styleeditormediator::NAME);
}