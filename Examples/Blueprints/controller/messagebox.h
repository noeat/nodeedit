#pragma once
#include "pmvcpp.h"

class messagebox : public PureMVC::SimpleCommand
{
public:
	void execute(PureMVC::INotification* note);
};