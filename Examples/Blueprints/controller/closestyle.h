#pragma once
#include "pmvcpp.h"

class closestylecmd : public PureMVC::SimpleCommand
{
public:
	void execute(PureMVC::INotification* note);
};