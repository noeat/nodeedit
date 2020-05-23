#pragma once
#include "pmvcpp.h"

class mainmediator : public PureMVC::Mediator
{
public:
	mainmediator();

	std::vector<int> listNotificationInterests() override;

	void handleNotification(PureMVC::INotification* notification) override;

	void onRegister() override;

	void onRemove() override;

	static const std::string NAME;

private:
	char buff_[260];
};