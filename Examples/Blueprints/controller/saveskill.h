#pragma once
#include "pmvcpp.h"

class saveskill : public PureMVC::SimpleCommand
{
public:
	void execute(PureMVC::INotification* note);
};