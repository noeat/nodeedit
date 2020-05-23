#pragma once
#include "pmvcpp.h"

class display : public PureMVC::SimpleCommand
{
public:
	void execute(PureMVC::INotification* note);
};