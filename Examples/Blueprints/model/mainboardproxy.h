#pragma once
#include "pmvcpp.h"
#include "define.h"

class mainboardproxy : public PureMVC::Proxy
{
public:
	mainboardproxy();

	void onRegister() override;
	void onRemove() override;

	bool& showstyleeditor() { return showstyleeditor_; }
	bool showstyleeditor() const { return showstyleeditor_; }

	Node* addnode(const ConfNode* conf, const ImVec2& pos);
	bool  removenode(Node* node);

	int next() const { return ++this->next_; }
	const static std::string NAME;

	const std::map<int, Node*>& nodes() const { return nodes_; }
	const std::vector<Node*>& skills() const { return skills_; }

	Node* getnode(int id) { return nodes_.find(id) == nodes_.end() ? nullptr : nodes_[id]; }
	const Node* getnode(int id) const { return const_cast<mainboardproxy*>(this)->getnode(id); }

	void registerPin(Node* node);
	void unregisterPin(const Node* node);
	Pin* GetPin(int pinid);
	
private:
	bool showstyleeditor_;
	mutable int  next_;
	std::map<int, Node*> nodes_;
	std::map<int, Pin*> pins_;
	std::vector<Node*> skills_;
};