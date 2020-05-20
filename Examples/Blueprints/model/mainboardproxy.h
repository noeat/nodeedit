#pragma once
#include "pmvcpp.h"

class mainboardproxy : public PureMVC::Proxy
{
public:
	mainboardproxy();

	void onRegister() override;
	void onRemove() override;

	bool& showstyleeditor() { return showstyleeditor_; }
	bool showstyleeditor() const { return showstyleeditor_; }


	const static std::string NAME;
private:
	bool showstyleeditor_;
};