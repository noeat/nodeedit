#pragma once
#include "pmvcpp.h"

class setting : public PureMVC::SimpleCommand
{
public:
	void execute(PureMVC::INotification* note);
};