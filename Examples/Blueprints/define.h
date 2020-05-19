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

#include "imgui_node_editor.h"
namespace ed = ax::NodeEditor;