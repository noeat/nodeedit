#pragma once
#include "pmvcpp.h"
#include <set>
#include <chrono>
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
	std::set<int> flows_;
	std::chrono::steady_clock::time_point flowtime_;
};