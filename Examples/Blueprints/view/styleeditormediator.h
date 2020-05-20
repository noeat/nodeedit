#pragma once
#include "pmvcpp.h"

class styleeditormediator : public PureMVC::Mediator
{
public:
	styleeditormediator();
	
	std::vector<int> listNotificationInterests() override;

	void handleNotification(PureMVC::INotification* notification) override;
	
	void onRegister() override;
	
	void onRemove() override;

	void show();

	static const std::string NAME;

private:
	bool* showstyle_;
};