#include "showstyle.h"
#include "model/mainboardproxy.h"
#include "view/styleeditormediator.h"
#include <assert.h>
#include "define.h"

void showstylecmd::execute(PureMVC::INotification* note)
{
	PureMVC::IFacade* facade = this->getFacade();
	if (!facade->hasMediator(styleeditormediator::NAME))
	{
		auto* mediator = new styleeditormediator();
		facade->registerMediator(mediator);
		
		mainboardproxy* maindata = dynamic_cast<mainboardproxy*>(
			facade->retrieveProxy(mainboardproxy::NAME));		
		assert(maindata != nullptr);
		maindata->showstyleeditor() = true;
		facade->sendNotification(COMMANDTYPE::SHOWSTYPE, (void*)&maindata->showstyleeditor());
	}
}