#pragma once
enum class PinType
{
	Flow,
	Bool,
	Int,
	Float,
	String,
	Object,
	Function,
	Delegate,
};

enum class PinKind
{
	Output,
	Input
};

enum class NodeType
{
	Blueprint,
	Simple,
	Tree,
	Comment,
	Houdini
};


enum COMMANDTYPE 
{
	NONE,
	STARTUP,
	SETTING,
	DISPLAY,
	DISPLAYLEFTPANE,
	SHOWSTYLEBTN,
	OPENSTYLE,
	CLOSESTYLE,
	SHOWSTYPE,
	CHECKSTYLECLOSE,
	LAODING,
	DISPLAYMAIN,
	MAINMENU,
	DISPLAYMAINMENU,
	MAINMENUCLICK,
	DISPLAYNODE = 1000,
};

enum NODETYPE 
{
	NODETYPE_NONE,
	ENTRY,
	SEQUENCE = 100,
	PARALLEL = 101,
	BRANCH = 103,
	LISTEN = 104,
	REPEATED = 200,
	WAIT = 201,
	EVENTWAIT = 202,
	EVENTADPTOR = 203,
	ACTION = 300,
};

#define UISTR_LEN 256
typedef char ui_string[UISTR_LEN];

#include <vector>
#include "imgui_node_editor.h"
#include <variant>
namespace ed = ax::NodeEditor;

struct ConfPin
{
	std::string		name;
	int				type;
	std::string		comment;
};

struct ConfNode
{
	std::string		name;
	int				type;
	std::string		comment;
	std::vector<ConfPin> inputs;
	std::vector<ConfPin> outputs;
};

union Value
{
	int int_;
	double double_;
	ui_string str_;
};


struct Node;
struct Pin
{
	ed::PinId id;
	::Node*   node;
	ui_string name;
	ui_string comment;
	PinType   type;
	PinKind   kind;
	Value	 value;
	std::vector<Pin*> links;
};

struct Node
{
	ed::NodeId id;
	ui_string  name;
	ui_string  comment;
	ImColor    color;
	int		   type;
	ImVec2	   size;
	bool	   show;
	ImVec2	   position;
	std::vector<Pin> inputs;
	std::vector<Pin> outputs;
};
