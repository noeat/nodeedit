#include "messagebox.h"
#include "define.h"
#include "view/messagedialogmediator.h"
#include "model/language.h"

void messagebox::execute(PureMVC::INotification* note)
{
	PureMVC::IFacade *facade = this->getFacade();
	std::pair<const char*, const char*>* args =
		static_cast<std::pair<const char*, const char*>*>(note->getBody());
	assert(args);

	PureMVC::IMediator* mediator = nullptr;
	if (facade->hasMediator(messagedialogmediator::NAME))
	{
		mediator = facade->retrieveMediator(messagedialogmediator::NAME);
	}
	else
	{
		mediator = new messagedialogmediator();
		facade->registerMediator(mediator);
	}
	
	messagedialogmediator* message = static_cast<messagedialogmediator*>(mediator);
	message->setTitle(args->first);
	message->setMessage(args->second);
}