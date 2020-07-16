#pragma once
#include "pmvcpp.h"

class waitmediator : public PureMVC::Mediator
{
public:
	waitmediator(int nodeid);

	std::vector<int> listNotificationInterests() override;

	void handleNotification(PureMVC::INotification* notification) override;

	void onRegister() override;

	void onRemove() override;

private:
	int nodeid_;
};