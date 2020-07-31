#pragma once
#include "pmvcpp.h"

class messagedialogmediator : public PureMVC::Mediator
{
public:
	messagedialogmediator();

	std::vector<int> listNotificationInterests() override;

	void handleNotification(PureMVC::INotification* notification) override;

	void onRegister() override;

	void onRemove() override;

	void setTitle(const char* title) { title_ = title; }

	void setMessage(const char* message) { message_ = message; }
	const static std::string NAME;
private:
	std::string title_;
	std::string message_;
};