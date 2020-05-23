#pragma once
#include "pmvcpp.h"

class loading : public PureMVC::SimpleCommand
{
public:
	void execute(PureMVC::INotification* note);

private:
	void loadworkspace(const char* dir);
};