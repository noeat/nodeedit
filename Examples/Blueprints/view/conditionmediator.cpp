#include "conditionmediator.h"
#include "define.h"
#include "imgui_node_editor.h"
#include "model/language.h"
#include "common.h"
#include <ax/Builders.h>
#include <ax/Widgets.h>
#include <ax/Drawing.h>
#include "imgui_internal.h"
namespace util = ax::NodeEditor::Utilities;
static const int            s_PinIconSize = 14;
ImColor GetIconColor(PinType type);


void DrawPinIcon(const Pin& pin, bool connected, int alpha);

conditionmediator::conditionmediator(int nodeid)
	:PureMVC::Mediator(id2mediatorname(nodeid)), nodeid_(nodeid)
{
	
}

std::vector<int> conditionmediator::listNotificationInterests()
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

void conditionmediator::handleNotification(PureMVC::INotification* notification)
{
	std::pair<util::BlueprintNodeBuilder*, Node*> *item = 
		(std::pair<util::BlueprintNodeBuilder*, Node*>*)notification->getBody();
	PureMVC::IFacade* facade = this->getFacade();
	auto builder = item->first;
	auto node = item->second;
	assert(node != nullptr && node->type == NODETYPE::BRANCH);
	builder->Begin(node->id);	
	builder->Header(node->color);		
	ImGui::Spring(0); 
	HelpMarker(node->comment);
	ImGui::TextUnformatted(node->name);	
	ImGui::Spring(1);
	ImGui::Dummy(ImVec2(0, 14));
	ImGui::Spring(2);
	ImGui::Spring(3);
	builder->EndHeader();
	
	auto alpha = ImGui::GetStyle().Alpha;

	for (auto& pin : node->inputs)
	{
		builder->Input(pin.id);
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);
		DrawPinIcon(pin, false, (int)(alpha * 255));
		HelpMarker2(pin.comment);
		if (pin.type == PinType::Bool)
		{
			if (pin.links.empty())
			{
				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(3, 2));
				ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(3, 2));
				ImGui::Checkbox(pin.name, &pin.value.bool_);
				ImGui::PopStyleVar(2);
			}
			else
			{
				ImGui::TextUnformatted(pin.name, ImGui::FindRenderedTextEnd(pin.name));
			}			
		}
		ImGui::Spring(0);
		ImGui::PopStyleVar();
		builder->EndInput();
	}
	
		
	for (auto& output : node->outputs)
	{
		builder->Output(output.id);
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);
		ImGui::TextUnformatted(output.name, ImGui::FindRenderedTextEnd(output.name));
		DrawPinIcon(output, false, (int)(alpha * 255));
		HelpMarker2(output.comment);
		ImGui::PopStyleVar();
		builder->EndOutput();
	}

	builder->End();	
}


void conditionmediator::onRegister()
{
	
}

void conditionmediator::onRemove()
{
	
}