#pragma once
#include "pmvcpp.h"

class showstylecmd : public PureMVC::SimpleCommand
{
public:
	void execute(PureMVC::INotification* note);
};