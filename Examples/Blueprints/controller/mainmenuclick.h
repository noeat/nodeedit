#pragma once
#include "pmvcpp.h"

class mainmenuclick : public PureMVC::SimpleCommand
{
public:
	void execute(PureMVC::INotification* note);
};