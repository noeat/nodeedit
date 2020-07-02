#pragma once
#include "pmvcpp.h"

class addpin : public PureMVC::SimpleCommand
{
public:
	void execute(PureMVC::INotification* note);
};