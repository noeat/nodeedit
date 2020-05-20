#pragma once
#pragma once
#include "pmvcpp.h"
#include <string>

class leftpanemediator : public PureMVC::Mediator
{
public:
	leftpanemediator();

	std::vector<int> listNotificationInterests() override;

	void handleNotification(PureMVC::INotification* notification) override;

	void onRegister() override;
	
	virtual void onRemove() override;

	static const std::string NAME;

private:
	bool* showstyle_;
};