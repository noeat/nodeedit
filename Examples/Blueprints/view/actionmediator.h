#pragma once
#include "pmvcpp.h"

class actionmediator : public PureMVC::Mediator
{
public:
	actionmediator(int nodeid);

	std::vector<int> listNotificationInterests() override;

	void handleNotification(PureMVC::INotification* notification) override;

	void onRegister() override;

	void onRemove() override;

private:
	int nodeid_;
};