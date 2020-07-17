#include "saveskill.h"
#include "model/settingproxy.h"
#include "define.h"
#include "view/leftpanemediator.h"
#include "model/mainboardproxy.h"
#include "view/workspacemediator.h"
#include "model/language.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include <unordered_map>
#include <list>

class serializenode
{
public:
	virtual std::vector<Node*> serializ(Node*, rapidjson::Document& doc, const std::unordered_map<int, int>& idict);
};

class entryserialize : public serializenode
{
public:
	std::vector<Node*> serializ(Node* node, rapidjson::Document& doc, const std::unordered_map<int, int>& idict) override
	{
		std::vector<Node*> nodes;
		rapidjson::Value& jsonnode = doc["node_1"];
		jsonnode["id"] = 1;
		jsonnode["type"] = NODETYPE::ENTRY;
		if (!node->outputs[0].links.empty())
		{
			nodes.push_back(node->outputs[0].links[0]->node);
			assert(idict.find(node->outputs[0].links[0]->node->id.Get()) != idict.end());
			jsonnode["flow"] = idict.find(node->outputs[0].links[0]->node->id.Get())->second;
		}

		return nodes;
	}
};

class sequenceserialize : public serializenode
{
public:
	std::vector<Node*> serializ(Node* node, rapidjson::Document& doc, const std::unordered_map<int, int>& idict) override
	{
		std::vector<Node*> nodes;
		assert(idict.find(node->id.Get()) != idict.end());
		int dictid = idict.find(node->id.Get())->second;
		rapidjson::Value& jsonnode = doc[std::string("node_") + std::to_string(dictid)];
		jsonnode["id"] = dictid;
		jsonnode["type"] = NODETYPE::SEQUENCE;
	}
};

void saveskill::execute(PureMVC::INotification* note)
{
	PureMVC::IFacade *facade = this->getFacade();
	Node* node = static_cast<Node*>(note->getBody());
	mainboardproxy* proxy = dynamic_cast<mainboardproxy*>(
		facade->retrieveProxy(mainboardproxy::NAME));
	assert(proxy);
	rapidjson::Document doc;
	int id = 1;
	std::unordered_map<int, int> idict;
	std::list<Node*> open;
	open.push_back(node);
	while (!open.empty())
	{
		auto curr = open.front();
		idict[curr->id.Get()] = id++;
		open.pop_front();
		for (auto& output : curr->outputs)
		{
			for (auto& pin : output.links)
			{
				if (idict.count(pin->node->id.Get()) == 0)
				{
					open.push_back(pin->node);
				}
			}
		}
	}
	assert(node->skillid > 0);
	doc["skillid"] = node->skillid;
	doc["entryid"] = 1;	
}