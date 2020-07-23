#pragma once
#include "pmvcpp.h"

class openskill : public PureMVC::SimpleCommand
{
public:
	void execute(PureMVC::INotification* note);
};