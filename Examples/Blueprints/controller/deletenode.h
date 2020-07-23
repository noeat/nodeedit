#pragma once
#include "pmvcpp.h"

class deletenode : public PureMVC::SimpleCommand
{
public:
	void execute(PureMVC::INotification* note);
};