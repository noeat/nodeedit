#pragma once
#include "pmvcpp.h"

class listenmediator : public PureMVC::Mediator
{
public:
	listenmediator(int nodeid);

	std::vector<int> listNotificationInterests() override;

	void handleNotification(PureMVC::INotification* notification) override;

	void onRegister() override;

	void onRemove() override;

private:
	int nodeid_;
};