#pragma once
#include "pmvcpp.h"

class deletemediator : public PureMVC::Mediator
{
public:
	deletemediator();

	std::vector<int> listNotificationInterests() override;

	void handleNotification(PureMVC::INotification* notification) override;

	void onRegister() override;

	void onRemove() override;

	static const std::string NAME;

private:
	char buff_[260];
};