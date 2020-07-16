#include "eventwaitmediator.h"
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

eventwaitmediator::eventwaitmediator(int nodeid)
	:PureMVC::Mediator(id2mediatorname(nodeid)), nodeid_(nodeid)
{
	
}

std::vector<int> eventwaitmediator::listNotificationInterests()
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

void eventwaitmediator::handleNotification(PureMVC::INotification* notification)
{
	std::pair<util::BlueprintNodeBuilder*, Node*> *item = 
		(std::pair<util::BlueprintNodeBuilder*, Node*>*)notification->getBody();
	PureMVC::IFacade* facade = this->getFacade();
	auto builder = item->first;
	auto node = item->second;
	assert(node != nullptr && node->type == NODETYPE::EVENTWAIT);
	builder->Begin(node->id);	
	builder->Header(node->color);		
	ImGui::Spring(0); 
	HelpMarker(node->comment);
	ImGui::TextUnformatted(node->name);	
	ImGui::Dummy(ImVec2(0, 20));
	ImGui::Spring(1);

	builder->EndHeader();
	
	auto alpha = ImGui::GetStyle().Alpha;

	builder->Input(node->inputs[0].id);
	ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);
	DrawPinIcon(node->inputs[0], false, (int)(alpha * 255));
	HelpMarker2(node->inputs[0].comment);
	ImGui::Spring(0);
	ImGui::PopStyleVar();
	builder->EndInput();

	builder->Input(node->inputs[1].id);
	ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);
	DrawPinIcon(node->inputs[1], false, (int)(alpha * 255));
	HelpMarker2(node->inputs[1].comment);
	if (node->inputs[1].links.empty())
	{
		ImGui::PushItemWidth(80);
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(1, 1));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(1, 1));
		ImGui::InputText(node->inputs[1].name, node->inputs[1].value.str_, sizeof(node->inputs[1].value.str_));
		ImGui::PopStyleVar(2);
		ImGui::PopItemWidth();
	}
	else
	{
		ImGui::TextUnformatted(node->inputs[1].name, ImGui::FindRenderedTextEnd(node->inputs[1].name));
	}
	ImGui::Spring(0);
	ImGui::PopStyleVar();
	builder->EndInput();

	builder->Input(node->inputs[2].id);
	ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);
	DrawPinIcon(node->inputs[2], false, (int)(alpha * 255));
	HelpMarker2(node->inputs[2].comment);
	ImGui::TextUnformatted(node->inputs[2].name, ImGui::FindRenderedTextEnd(node->inputs[2].name));
	ImGui::Spring(0);
	ImGui::PopStyleVar();
	builder->EndInput();
		
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


void eventwaitmediator::onRegister()
{
	
}

void eventwaitmediator::onRemove()
{
	
}