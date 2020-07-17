#include "leftpanemediator.h"
#include "define.h"
#include "imgui_node_editor.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui_internal.h>

static bool Splitter(bool split_vertically, float thickness, float* size1, float* size2, float min_size1, float min_size2, float splitter_long_axis_size = -1.0f)
{
	using namespace ImGui;
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = g.CurrentWindow;
	ImGuiID id = window->GetID("##Splitter");
	ImRect bb;
	bb.Min = window->DC.CursorPos + (split_vertically ? ImVec2(*size1, 0.0f) : ImVec2(0.0f, *size1));
	bb.Max = bb.Min + CalcItemSize(split_vertically ? ImVec2(thickness, splitter_long_axis_size) : ImVec2(splitter_long_axis_size, thickness), 0.0f, 0.0f);
	return SplitterBehavior(bb, id, split_vertically ? ImGuiAxis_X : ImGuiAxis_Y, size1, size2, min_size1, min_size2, 0.0f);
}

extern const int            s_PinIconSize;
extern ImTextureID          s_HeaderBackground;
extern ImTextureID          s_SaveIcon;
extern ImTextureID          s_RestoreIcon;

const std::string leftpanemediator::NAME = "leftpanemediator"
;
leftpanemediator::leftpanemediator() :
	PureMVC::Mediator(leftpanemediator::NAME)
{
	
}

std::vector<int> leftpanemediator::listNotificationInterests()
{
	return std::vector<int>{COMMANDTYPE::DISPLAYLEFTPANE};
}

void leftpanemediator::handleNotification(PureMVC::INotification* notification)
{	
	static float leftPaneWidth = 400.0f;
	static float rightPaneWidth = 800.0f;
	Splitter(true, 4.0f, &leftPaneWidth, &rightPaneWidth, 50.0f, 50.0f);
	float leftpane = leftPaneWidth - 4.0;
	PureMVC::IFacade* facade = this->getFacade();
	ImGui::BeginChild("Selection", ImVec2(leftpane, 0));
	leftpane = ImGui::GetContentRegionAvailWidth();
	//static bool showStyleEditor = false;
	ImGui::BeginHorizontal("Style Editor", ImVec2(leftpane, 0));
	ImGui::Spring(0.0f, 0.0f);
	if (ImGui::Button("Zoom to Content"))
		ed::NavigateToContent();
	
	ImGui::Spring(0.0f);
	if (ImGui::Button("Show Flow"))
	{
		facade->sendNotification(COMMANDTYPE::SHOWFLOW);
	}

	ImGui::Spring();
	if (ImGui::Button("Edit Style"))
	{
		facade->sendNotification(COMMANDTYPE::OPENSTYLE);
	}
	
	ImGui::EndHorizontal();
	ImGui::EndChild();	
}

void leftpanemediator::onRegister()
{

}

void leftpanemediator::onRemove()
{

}
