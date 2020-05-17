#pragma once
#include "pmvcpp.h"

class startup : public PureMVC::SimpleCommand
{
public:
	void execute(PureMVC::INotification* note);
};