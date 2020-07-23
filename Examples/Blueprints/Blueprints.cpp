#include "Application.h"
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <utility>
#include <imgui_node_editor.h>
#include <ax/Math2D.h>
#include <ax/Builders.h>
#include <ax/Widgets.h>
#include "pmvcpp.h"
#include "controller/startup.h"
#include "controller/setting.h"
#include "controller/closestyle.h"
#include "controller/showstyle.h"
#include "controller/loading.h"
#include "controller/display.h"
#include "controller/mainmeun.h"
#include "controller/mainmenuclick.h"
#include "controller/addpin.h"
#include "controller/saveskill.h"
#include "controller/openskill.h"
#include "common.h"
#include "define.h"


#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui_internal.h>

using namespace PureMVC;

static inline ImRect ImGui_GetItemRect()
{
    return ImRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax());
}

static inline ImRect ImRect_Expanded(const ImRect& rect, float x, float y)
{
    auto result = rect;
    result.Min.x -= x;
    result.Min.y -= y;
    result.Max.x += x;
    result.Max.y += y;
    return result;
}

namespace ed = ax::NodeEditor;
namespace util = ax::NodeEditor::Utilities;

using namespace ax;

using ax::Widgets::IconType;

static ed::EditorContext* m_Editor = nullptr;

struct NodeIdLess
{
    bool operator()(const ed::NodeId& lhs, const ed::NodeId& rhs) const
    {
        return lhs.AsPointer() < rhs.AsPointer();
    }
};

static const float          s_TouchTime = 1.0f;
static std::map<ed::NodeId, float, NodeIdLess> s_NodeTouchTime;


static void TouchNode(ed::NodeId id)
{
    s_NodeTouchTime[id] = s_TouchTime;
}

static float GetTouchProgress(ed::NodeId id)
{
    auto it = s_NodeTouchTime.find(id);
    if (it != s_NodeTouchTime.end() && it->second > 0.0f)
        return (s_TouchTime - it->second) / s_TouchTime;
    else
        return 0.0f;
}

static void UpdateTouch()
{
    const auto deltaTime = ImGui::GetIO().DeltaTime;
    for (auto& entry : s_NodeTouchTime)
    {
        if (entry.second > 0.0f)
            entry.second -= deltaTime;
    }
}

const char* Application_GetName()
{
    return "Blueprints";
}

void Application_Initialize()
{
    ed::Config config;

    config.SettingsFile = "Blueprints.json";

    m_Editor = ed::CreateEditor(&config);
    ed::SetCurrentEditor(m_Editor);

	Facade* facade = Facade::getInstance("root");
	facade->registerCommand<startup>(COMMANDTYPE::STARTUP);
	facade->registerCommand<closestylecmd>(COMMANDTYPE::CLOSESTYLE);
	facade->registerCommand<showstylecmd>(COMMANDTYPE::OPENSTYLE);
	facade->registerCommand<loading>(COMMANDTYPE::LAODING);
	facade->registerCommand<display>(COMMANDTYPE::DISPLAY);
	facade->registerCommand<mainmeun>(COMMANDTYPE::MAINMENU);
	facade->registerCommand<mainmenuclick>(COMMANDTYPE::MAINMENUCLICK);
	facade->registerCommand<addpin>(COMMANDTYPE::ADDPIN);
	facade->registerCommand<saveskill>(COMMANDTYPE::SAVESKILL);
	facade->registerCommand<openskill>(COMMANDTYPE::OPENSKILL);
	facade->sendNotification(COMMANDTYPE::STARTUP);
}

void Application_Finalize()
{
    auto releaseTexture = [](ImTextureID& id)
    {
        if (id)
        {
            Application_DestroyTexture(id);
            id = nullptr;
        }
    };

    if (m_Editor)
    {
        ed::DestroyEditor(m_Editor);
        m_Editor = nullptr;
    }
}


void Application_Frame()
{
    UpdateTouch();

    auto& io = ImGui::GetIO();

    ImGui::Text("FPS: %.2f (%.2gms)", io.Framerate, io.Framerate ? 1000.0f / io.Framerate : 0.0f);

    ed::SetCurrentEditor(m_Editor);

	Facade* facade = Facade::getInstance("root");
	facade->sendNotification(COMMANDTYPE::DISPLAY);
}

