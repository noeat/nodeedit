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

	int next() const { return ++this->next_; }
	const static std::string NAME;
private:
	bool showstyleeditor_;
	mutable int  next_;
};