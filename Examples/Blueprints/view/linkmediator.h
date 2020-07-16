#pragma once
#include "pmvcpp.h"

class linkmediator : public PureMVC::Mediator
{
public:
	linkmediator();

	std::vector<int> listNotificationInterests() override;

	void handleNotification(PureMVC::INotification* notification) override;

	void onRegister() override;

	void onRemove() override;

	static const std::string NAME;

private:
	char buff_[260];
};