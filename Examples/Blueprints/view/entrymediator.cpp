#include "entrymediator.h"
#include "define.h"
#include "imgui_node_editor.h"
#include "model/language.h"
#include "common.h"
#include <ax/Builders.h>
#include <ax/Widgets.h>
#include <ax/Drawing.h>
namespace util = ax::NodeEditor::Utilities;
static const int            s_PinIconSize = 14;
ImColor GetIconColor(PinType type)
{
	switch (type)
	{
	default:
	case PinType::Flow:     return ImColor(255, 255, 255);
	case PinType::Bool:     return ImColor(220, 48, 48);
	case PinType::Int:      return ImColor(68, 201, 156);
	case PinType::Float:    return ImColor(147, 226, 74);
	case PinType::String:   return ImColor(124, 21, 153);
	case PinType::Object:   return ImColor(51, 150, 215);
	case PinType::Function: return ImColor(218, 0, 183);
	case PinType::Delegate: return ImColor(255, 48, 48);
	}
};

void DrawPinIcon(const Pin& pin, bool connected, int alpha)
{
	ax::Drawing::IconType iconType;
	ImColor  color = GetIconColor(pin.type);
	color.Value.w = alpha / 255.0f;
	switch (pin.type)
	{
	case PinType::Flow:     iconType = ax::Drawing::IconType::Flow;   break;
	case PinType::Bool:     iconType = ax::Drawing::IconType::Circle; break;
	case PinType::Int:      iconType = ax::Drawing::IconType::Circle; break;
	case PinType::Float:    iconType = ax::Drawing::IconType::Circle; break;
	case PinType::String:   iconType = ax::Drawing::IconType::Circle; break;
	case PinType::Object:   iconType = ax::Drawing::IconType::Circle; break;
	case PinType::Function: iconType = ax::Drawing::IconType::Circle; break;
	case PinType::Delegate: iconType = ax::Drawing::IconType::Square; break;
	default:
		return;
	}

	ax::Widgets::Icon(ImVec2(s_PinIconSize, s_PinIconSize), iconType, connected, color, ImColor(32, 32, 32, alpha));
};
entrymediator::entrymediator(int nodeid)
	:PureMVC::Mediator(id2mediatorname(nodeid)), nodeid_(nodeid)
{
	
}

std::vector<int> entrymediator::listNotificationInterests()
{
	return std::vector<int>{COMMANDTYPE::DISPLAYNODE + this->nodeid_};
}


static void HelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
	{
		ed::Suspend();
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip(); ed::Resume();
	}
}

static void HelpMarker2(const char* desc)
{
	if (ImGui::IsItemHovered())
	{
		ed::Suspend();
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip(); ed::Resume();
	}
}

static char buff[20];
static void SkillIDTip(int skillid)
{
	if (ImGui::IsItemHovered())
	{
		ed::Suspend();
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		snprintf(buff, 20, "%d", skillid);
		ImGui::TextUnformatted(buff);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip(); ed::Resume();
	}
}

void entrymediator::handleNotification(PureMVC::INotification* notification)
{
	std::pair<util::BlueprintNodeBuilder*, Node*> *item = 
		(std::pair<util::BlueprintNodeBuilder*, Node*>*)notification->getBody();
	PureMVC::IFacade* facade = this->getFacade();
	auto builder = item->first;
	auto node = item->second;
	assert(node != nullptr && node->type == NODETYPE::ENTRY);		
	builder->Begin(node->id);
	builder->Header(node->color);
	ImGui::Spring(0);
	HelpMarker(node->comment);
	//ImGui::TextUnformatted(node->name);
	ImGui::PushItemWidth(30);
	ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
	ImGui::DragInt("", &node->skillid, 1, 0, 100000, node->name);
	ImGui::PopTextWrapPos();
	ImGui::PopItemWidth();
	SkillIDTip(node->skillid);
	//HelpMarker(node->comment);
	ImGui::Spring(1);
	ImGui::Dummy(ImVec2(0, 14));
	ImGui::Spring(2);
	builder->EndHeader();
	auto alpha = ImGui::GetStyle().Alpha;	
	ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);
	builder->Output(node->outputs[0].id);	
	DrawPinIcon(node->outputs[0], false, (int)(alpha * 255));
	//HelpMarker2(node->outputs[0].comment);
	ImGui::PopStyleVar();
	builder->EndOutput();
	builder->End();
}


void entrymediator::onRegister()
{
	
}

void entrymediator::onRemove()
{
	
}