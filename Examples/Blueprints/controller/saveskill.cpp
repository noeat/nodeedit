#include "saveskill.h"
#include "model/settingproxy.h"
#include "define.h"
#include "view/leftpanemediator.h"
#include "model/mainboardproxy.h"
#include "view/workspacemediator.h"
#include "model/language.h"
#include "model/settingproxy.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include <unordered_map>
#include <map>
#include <list>
#include <rapidjson/writer.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/prettywriter.h>
#include <fstream>
#include "utiliti.h"
#include <algorithm>

class serializenode
{
public:
	virtual std::vector<Node*> serializ(Node*, rapidjson::Document& doc, const std::unordered_map<int, int>& idict) = 0;
};

class entryserialize : public serializenode
{
public:
	std::vector<Node*> serializ(Node* node, rapidjson::Document& doc, const std::unordered_map<int, int>& idict) override
	{
		std::vector<Node*> nodes;
		rapidjson::Value jsonnode;
		jsonnode.SetObject();		

		{
			rapidjson::Value val;
			val.SetInt(1);
			jsonnode.AddMember("id", val, doc.GetAllocator());
		}

		{
			rapidjson::Value val;
			val.SetInt(NODETYPE::ENTRY);
			jsonnode.AddMember("type", val, doc.GetAllocator());
		}
	
		if (!node->outputs[0].links.empty())
		{
			nodes.push_back(node->outputs[0].links[0]->node);
			assert(idict.find(node->outputs[0].links[0]->node->id.Get()) != idict.end());
			rapidjson::Value val;
			val.SetInt(idict.find(node->outputs[0].links[0]->node->id.Get())->second);
			jsonnode.AddMember("flow", val, doc.GetAllocator());
		}
		doc.AddMember("node_1", jsonnode, doc.GetAllocator());
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
		rapidjson::Value jsonnode;
		jsonnode.SetObject();
		
		{
			rapidjson::Value val;
			val.SetInt(dictid);
			jsonnode.AddMember("id", val, doc.GetAllocator());
		}

		{
			rapidjson::Value val;
			val.SetInt(node->type);
			jsonnode.AddMember("type", val, doc.GetAllocator());
		}

	
		if (node->outputs.size() > 0)
		{
			rapidjson::Value jsonflows;
			jsonflows.SetArray();
			for (auto& output : node->outputs)
			{
				if (!output.links.empty())
				{
					nodes.push_back(output.links[0]->node);
					assert(idict.find(output.links[0]->node->id.Get()) != idict.end());
					int id = idict.find(output.links[0]->node->id.Get())->second;
					jsonflows.PushBack(id, doc.GetAllocator());
				}
			}

			jsonnode.AddMember("flows", jsonflows, doc.GetAllocator());
		}
		
		rapidjson::Value key;
		key.SetString(std::string("node_") + std::to_string(dictid), doc.GetAllocator());
		doc.AddMember(key, jsonnode, doc.GetAllocator());
		return nodes;
	}
};

class branchserialize : public serializenode
{
public:
	std::vector<Node*> serializ(Node* node, rapidjson::Document& doc, const std::unordered_map<int, int>& idict) override
	{
		std::vector<Node*> nodes;
		assert(idict.find(node->id.Get()) != idict.end());
		int dictid = idict.find(node->id.Get())->second;
		rapidjson::Value jsonnode;
		jsonnode.SetObject();
		{
			rapidjson::Value val;
			val.SetInt(dictid);
			jsonnode.AddMember("id", val, doc.GetAllocator());
		}

		{
			rapidjson::Value val;
			val.SetInt(NODETYPE::BRANCH);
			jsonnode.AddMember("type", val, doc.GetAllocator());
		}
		
		if (node->inputs[1].links.empty())
		{
			rapidjson::Value val;
			val.SetBool(node->inputs[1].value.bool_);
			jsonnode.AddMember("condition", val, doc.GetAllocator());
		}

		assert(node->outputs.size() > 1);
		if (!node->outputs[0].links.empty())
		{
			assert(idict.find(node->outputs[0].links[0]->node->id.Get()) != idict.end());
			rapidjson::Value val;
			val.SetInt(idict.find(node->outputs[0].links[0]->node->id.Get())->second);
			jsonnode.AddMember("trueflow", val, doc.GetAllocator());
			nodes.push_back(node->outputs[0].links[0]->node);
		}

		if (!node->outputs[1].links.empty())
		{
			assert(idict.find(node->outputs[1].links[0]->node->id.Get()) != idict.end());
			rapidjson::Value val;
			val.SetInt(idict.find(node->outputs[1].links[0]->node->id.Get())->second);
			jsonnode.AddMember("falseflow", val, doc.GetAllocator());
			nodes.push_back(node->outputs[1].links[0]->node);
		}

		rapidjson::Value key;
		key.SetString(std::string("node_") + std::to_string(dictid), doc.GetAllocator());
		doc.AddMember(key, jsonnode, doc.GetAllocator());
		return nodes;
	}
};

class listenserialize : public serializenode
{
public:
	std::vector<Node*> serializ(Node* node, rapidjson::Document& doc, const std::unordered_map<int, int>& idict) override
	{
		std::vector<Node*> nodes;
		assert(idict.find(node->id.Get()) != idict.end());
		int dictid = idict.find(node->id.Get())->second;
		rapidjson::Value jsonnode;
		jsonnode.SetObject();
		{
			rapidjson::Value val;
			val.SetInt(dictid);
			jsonnode.AddMember("id", val, doc.GetAllocator());
		}

		{
			rapidjson::Value val;
			val.SetInt(NODETYPE::LISTEN);
			jsonnode.AddMember("type", val, doc.GetAllocator());
		}

		if (node->inputs[1].links.empty())
		{
			rapidjson::Value val;
			val.SetString(node->inputs[1].value.str_, doc.GetAllocator());
			jsonnode.AddMember("event", val, doc.GetAllocator());
		}

		if (node->inputs[2].links.empty())
		{
			rapidjson::Value val;
			val.SetInt(0);
			jsonnode.AddMember("inst", val, doc.GetAllocator());
		}

		assert(node->outputs.size() > 1);
		if (!node->outputs[0].links.empty())
		{
			assert(idict.find(node->outputs[0].links[0]->node->id.Get()) != idict.end());
			rapidjson::Value val;
			val.SetInt(idict.find(node->outputs[0].links[0]->node->id.Get())->second);
			jsonnode.AddMember("flow", val, doc.GetAllocator());
			nodes.push_back(node->outputs[0].links[0]->node);
		}

		if (!node->outputs[1].links.empty())
		{
			assert(idict.find(node->outputs[1].links[0]->node->id.Get()) != idict.end());
			rapidjson::Value val;
			val.SetInt(idict.find(node->outputs[1].links[0]->node->id.Get())->second);
			jsonnode.AddMember("eventflow", val, doc.GetAllocator());
			nodes.push_back(node->outputs[1].links[0]->node);
		}

		rapidjson::Value key;
		key.SetString(std::string("node_") + std::to_string(dictid), doc.GetAllocator());
		doc.AddMember(key, jsonnode, doc.GetAllocator());
		return nodes;
	}
};

class eventadapterserlize : public serializenode
{
public:
	std::vector<Node*> serializ(Node* node, rapidjson::Document& doc, const std::unordered_map<int, int>& idict) override
	{
		std::vector<Node*> nodes;
		assert(idict.find(node->id.Get()) != idict.end());
		int dictid = idict.find(node->id.Get())->second;
		rapidjson::Value jsonnode;
		jsonnode.SetObject();
		{
			rapidjson::Value val;
			val.SetInt(dictid);
			jsonnode.AddMember("id", val, doc.GetAllocator());
		}

		{
			rapidjson::Value val;
			val.SetInt(NODETYPE::EVENTADPTOR);
			jsonnode.AddMember("type", val, doc.GetAllocator());
		}

		assert(node->outputs.size() > 1);
		if (!node->outputs[0].links.empty())
		{
			assert(idict.find(node->outputs[0].links[0]->node->id.Get()) != idict.end());
			rapidjson::Value val;
			val.SetInt(idict.find(node->outputs[0].links[0]->node->id.Get())->second);
			jsonnode.AddMember("flow", val, doc.GetAllocator());
			nodes.push_back(node->outputs[0].links[0]->node);
		}

		if (!node->outputs[1].links.empty())
		{
			rapidjson::Value jsnode;
			jsnode.SetArray();

			for (auto& link : node->outputs[1].links)
			{
				assert(idict.find(link->node->id.Get()) != idict.end());
				int nodeid = idict.find(link->node->id.Get())->second;
				int index = std::distance(link->node->inputs.begin(),
					std::find_if(link->node->inputs.begin(), link->node->inputs.end(), [link](const Pin& p)
				{
					return link->id.Get() == p.id.Get();
				}));

				std::string str = std::to_string(nodeid) + "|" + std::to_string(index);
				rapidjson::Value v;
				v.SetString(str, doc.GetAllocator());

				jsnode.PushBack(v, doc.GetAllocator());
			}			

			jsonnode.AddMember("outputs", jsnode, doc.GetAllocator());
		}

		rapidjson::Value key;
		key.SetString(std::string("node_") + std::to_string(dictid), doc.GetAllocator());
		doc.AddMember(key, jsonnode, doc.GetAllocator());
		return nodes;
	};
};

class repeatedserlize : public serializenode
{
public:
	std::vector<Node*> serializ(Node* node, rapidjson::Document& doc, const std::unordered_map<int, int>& idict) override
	{
		std::vector<Node*> nodes;
		assert(idict.find(node->id.Get()) != idict.end());
		int dictid = idict.find(node->id.Get())->second;
		rapidjson::Value jsonnode;
		jsonnode.SetObject();
		{
			rapidjson::Value val;
			val.SetInt(dictid);
			jsonnode.AddMember("id", val, doc.GetAllocator());
		}

		{
			rapidjson::Value val;
			val.SetInt(NODETYPE::REPEATED);
			jsonnode.AddMember("type", val, doc.GetAllocator());
		}

		if (node->inputs[1].links.empty())
		{
			rapidjson::Value val;
			val.SetInt(node->inputs[1].value.int_);
			jsonnode.AddMember("count", val, doc.GetAllocator());
		}

		assert(node->outputs.size() > 0);
		if (!node->outputs[0].links.empty())
		{
			assert(idict.find(node->outputs[0].links[0]->node->id.Get()) != idict.end());
			rapidjson::Value val;
			val.SetInt(idict.find(node->outputs[0].links[0]->node->id.Get())->second);
			jsonnode.AddMember("flow", val, doc.GetAllocator());
			nodes.push_back(node->outputs[0].links[0]->node);
		}

		rapidjson::Value key;
		key.SetString(std::string("node_") + std::to_string(dictid), doc.GetAllocator());
		doc.AddMember(key, jsonnode, doc.GetAllocator());
		return nodes;
	}
};

class waitserlize : public serializenode
{
public:
	std::vector<Node*> serializ(Node* node, rapidjson::Document& doc, const std::unordered_map<int, int>& idict) override
	{
		std::vector<Node*> nodes;
		assert(idict.find(node->id.Get()) != idict.end());
		int dictid = idict.find(node->id.Get())->second;
		rapidjson::Value jsonnode;
		jsonnode.SetObject();
		{
			rapidjson::Value val;
			val.SetInt(dictid);
			jsonnode.AddMember("id", val, doc.GetAllocator());
		}

		{
			rapidjson::Value val;
			val.SetInt(NODETYPE::WAIT);
			jsonnode.AddMember("type", val, doc.GetAllocator());
		}

		if (node->inputs[1].links.empty())
		{
			rapidjson::Value val;
			val.SetFloat(node->inputs[1].value.float_);
			jsonnode.AddMember("wait", val, doc.GetAllocator());
		}

		assert(node->outputs.size() > 0);
		if (!node->outputs[0].links.empty())
		{
			assert(idict.find(node->outputs[0].links[0]->node->id.Get()) != idict.end());
			rapidjson::Value val;
			val.SetInt(idict.find(node->outputs[0].links[0]->node->id.Get())->second);
			jsonnode.AddMember("flow", val, doc.GetAllocator());
			nodes.push_back(node->outputs[0].links[0]->node);
		}

		rapidjson::Value key;
		key.SetString(std::string("node_") + std::to_string(dictid), doc.GetAllocator());
		doc.AddMember(key, jsonnode, doc.GetAllocator());
		return nodes;
	}
};

class eventwaitserlize : public serializenode
{
public:
	std::vector<Node*> serializ(Node* node, rapidjson::Document& doc, const std::unordered_map<int, int>& idict) override
	{
		std::vector<Node*> nodes;
		assert(idict.find(node->id.Get()) != idict.end());
		int dictid = idict.find(node->id.Get())->second;
		rapidjson::Value jsonnode;
		jsonnode.SetObject();
		{
			rapidjson::Value val;
			val.SetInt(dictid);
			jsonnode.AddMember("id", val, doc.GetAllocator());
		}

		{
			rapidjson::Value val;
			val.SetInt(NODETYPE::EVENTWAIT);
			jsonnode.AddMember("type", val, doc.GetAllocator());
		}

		if (node->inputs[1].links.empty())
		{
			rapidjson::Value val;
			val.SetString(node->inputs[1].value.str_, doc.GetAllocator());
			jsonnode.AddMember("event", val, doc.GetAllocator());
		}

		if (node->inputs[2].links.empty())
		{
			rapidjson::Value val;
			val.SetInt(0);
			jsonnode.AddMember("inst", val, doc.GetAllocator());
		}

		assert(node->outputs.size() > 0);
		if (!node->outputs[0].links.empty())
		{
			assert(idict.find(node->outputs[0].links[0]->node->id.Get()) != idict.end());
			rapidjson::Value val;
			val.SetInt(idict.find(node->outputs[0].links[0]->node->id.Get())->second);
			jsonnode.AddMember("flow", val, doc.GetAllocator());
			nodes.push_back(node->outputs[0].links[0]->node);
		}

		rapidjson::Value key;
		key.SetString(std::string("node_") + std::to_string(dictid), doc.GetAllocator());
		doc.AddMember(key, jsonnode, doc.GetAllocator());
		return nodes;
	}
};

class actionserlize : public serializenode
{
public:
	std::vector<Node*> serializ(Node* node, rapidjson::Document& doc, const std::unordered_map<int, int>& idict) override
	{
		std::vector<Node*> nodes;
		assert(idict.find(node->id.Get()) != idict.end());
		int dictid = idict.find(node->id.Get())->second;
		rapidjson::Value jsonnode;
		jsonnode.SetObject();
		{
			rapidjson::Value val;
			val.SetInt(dictid);
			jsonnode.AddMember("id", val, doc.GetAllocator());
		}

		{
			rapidjson::Value val;
			val.SetInt(NODETYPE::ACTION);
			jsonnode.AddMember("type", val, doc.GetAllocator());
		}

		tokenizer t(node->name, '#');
		assert(t.size() > 2);

		rapidjson::Value fn;
		fn.SetString(t[2], doc.GetAllocator());
		jsonnode.AddMember("actionfn", fn, doc.GetAllocator());

		rapidjson::Value jsinput;
		jsinput.SetArray();
		for (int i = 1; i < node->inputs.size(); ++i)
		{
			auto input = node->inputs[i];
			tokenizer tok(input.name, '#');
			if (input.type == PinType::Bool)
			{
				jsinput.PushBack(input.value.bool_, doc.GetAllocator());
			}
			else if (input.type == PinType::Int)
			{
				jsinput.PushBack(input.value.int_, doc.GetAllocator());
			}
			else if (input.type == PinType::Float)
			{
				jsinput.PushBack(input.value.float_, doc.GetAllocator());
			}
			else if (input.type == PinType::String)
			{
				rapidjson::Value v;
				v.SetString(input.value.str_, doc.GetAllocator());
				jsinput.PushBack(v, doc.GetAllocator());
			}
			else
			{
				if (tok.size() > 2 && atoi(tok[2]) == 0)
				{
					jsinput.PushBack(0, doc.GetAllocator());
				}
				else if (tok.size() > 2 && atoi(tok[2]) == 1)
				{
					if (input.value.str_[0] != '\0')
					{
						rapidjson::Document checker;
						if (checker.Parse(input.value.str_).HasParseError())
						{
							PureMVC::Facade* facade = PureMVC::Facade::getInstance("root");
							std::pair<const char*, const char*> pair;
							pair.first = "Save Skill Error";
							char buff[1024];
							snprintf(buff, 1024, "[%s | %s]-> Parse: [ %s ], Error: [%d].", t[0], tok[0], input.value.str_, checker.GetParseError());
							pair.second = buff;
							facade->sendNotification(COMMANDTYPE::MESSAGEBOX, &pair);
							nodes.clear();
							nodes.push_back(nullptr);
							return nodes;
						}
						else
						{
							rapidjson::Value v2;
							v2.CopyFrom(checker, doc.GetAllocator());
							jsinput.PushBack(v2, doc.GetAllocator());
						}
					}
					else
					{
						rapidjson::Value v;
						v.SetNull();
						jsinput.PushBack(v, doc.GetAllocator());
					}
					
					//jsinput.PushBack(0, doc.GetAllocator());
				}
				else
				{
					rapidjson::Value v;
					v.SetNull();
					jsinput.PushBack(v, doc.GetAllocator());
				}
			}
		}
		jsonnode.AddMember("input", jsinput, doc.GetAllocator());

		if (!node->outputs.empty())
		{
			rapidjson::Value jsnode;
			jsnode.SetArray();
			int order = 0;
			for (auto& output : node->outputs)
			{
				++order;
				rapidjson::Value jsnode1;
				jsnode1.SetArray();
				for (auto& link : output.links)
				{
					assert(idict.find(link->node->id.Get()) != idict.end());
					int nodeid = idict.find(link->node->id.Get())->second;
					int index = std::distance(link->node->inputs.begin(),
						std::find_if(link->node->inputs.begin(), link->node->inputs.end(), [link](const Pin& p) 
					{
						return link->id.Get() == p.id.Get();
					}));

					std::string str = std::to_string(nodeid) + "|" + std::to_string(index);
					rapidjson::Value v;
					v.SetString(str, doc.GetAllocator());

					jsnode1.PushBack(v, doc.GetAllocator());
				}

				jsnode.PushBack(jsnode1, doc.GetAllocator());
			}

			jsonnode.AddMember("outputs", jsnode, doc.GetAllocator());
		}
		
		rapidjson::Value key;
		key.SetString(std::string("node_") + std::to_string(dictid), doc.GetAllocator());
		doc.AddMember(key, jsonnode, doc.GetAllocator());
		return nodes;
	}
};

static std::unordered_map<int, serializenode*> ss;
static bool init()
{
	ss[NODETYPE::ENTRY] = new entryserialize;
	ss[NODETYPE::SEQUENCE] = new sequenceserialize;
	ss[NODETYPE::PARALLEL] = new sequenceserialize;
	ss[NODETYPE::BRANCH] = new branchserialize;
	ss[NODETYPE::LISTEN] = new listenserialize;
	ss[NODETYPE::REPEATED] = new repeatedserlize;
	ss[NODETYPE::WAIT] = new waitserlize;
	ss[NODETYPE::EVENTWAIT] = new eventwaitserlize;
	ss[NODETYPE::EVENTADPTOR] = new eventadapterserlize;
	ss[NODETYPE::ACTION] = new actionserlize;
	return true;
}

const static bool _ = init();

void saveskill::execute(PureMVC::INotification* note)
{
	PureMVC::IFacade *facade = this->getFacade();
	Node* node = static_cast<Node*>(note->getBody());
	mainboardproxy* proxy = dynamic_cast<mainboardproxy*>(
		facade->retrieveProxy(mainboardproxy::NAME));
	assert(proxy);

	settingproxy* setting = dynamic_cast<settingproxy*>(
		facade->retrieveProxy(settingproxy::NAME));
	assert(setting);
	
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
	
	rapidjson::Document doc;
	doc.SetObject();
	{
		rapidjson::Value val;
		val.SetInt(node->skillid);
		doc.AddMember("skillid", val, doc.GetAllocator());
	}
	{
		rapidjson::Value val;
		val.SetInt(1);
		doc.AddMember("entryid", val, doc.GetAllocator());
	}
	
	std::vector<Node*> opend;
	opend.push_back(node);
	while (!opend.empty())
	{
		auto n = opend.back();
		opend.pop_back();
		if (n == nullptr)
		{
			node->saved = false;
			return;
		}
		auto items = ss[n->type]->serializ(n, doc, idict);
		opend.insert(opend.end(), items.begin(), items.end());
	}

	std::string filepath = setting->workspace() + "\\" + std::to_string(node->skillid) + ".json";
	std::ofstream ofs(filepath);
	rapidjson::OStreamWrapper osw(ofs);

	rapidjson::PrettyWriter<rapidjson::OStreamWrapper> writer(osw);
	doc.Accept(writer);
}