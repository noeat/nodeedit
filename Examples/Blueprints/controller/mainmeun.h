#pragma once
#include "pmvcpp.h"

class mainmeun : public PureMVC::SimpleCommand
{
public:
	void execute(PureMVC::INotification* note);
};