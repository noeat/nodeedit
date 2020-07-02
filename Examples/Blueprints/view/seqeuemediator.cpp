#include "seqeuemediator.h"
#include "define.h"
#include "imgui_node_editor.h"
#include "model/language.h"
#include "common.h"
#include <ax/Builders.h>
#include <ax/Widgets.h>
#include <ax/Drawing.h>
namespace util = ax::NodeEditor::Utilities;
static const int            s_PinIconSize = 14;
ImColor GetIconColor(PinType type);


void DrawPinIcon(const Pin& pin, bool connected, int alpha);

seqeuemediator::seqeuemediator(int nodeid)
	:PureMVC::Mediator(id2mediatorname(nodeid)), nodeid_(nodeid)
{
	
}

std::vector<int> seqeuemediator::listNotificationInterests()
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

void seqeuemediator::handleNotification(PureMVC::INotification* notification)
{
	std::pair<util::BlueprintNodeBuilder*, Node*> *item = 
		(std::pair<util::BlueprintNodeBuilder*, Node*>*)notification->getBody();
	PureMVC::IFacade* facade = this->getFacade();
	auto builder = item->first;
	auto node = item->second;
	assert(node != nullptr && (node->type == NODETYPE::SEQUENCE||node->type == NODETYPE::PARALLEL));
	builder->Begin(node->id);
	builder->Header(node->color);
	ImGui::Spring(0);
	//ImGui::TextUnformatted(node->name);
	//ImGui::PushItemWidth(60);
	HelpMarker(node->comment);
	ImGui::TextUnformatted(node->name);
	//ImGui::PopItemWidth();
	//HelpMarker(node->comment);
	ImGui::Spring(1);
	ImGui::Dummy(ImVec2(0, 14));
	ImGui::Spring(2);
	builder->EndHeader();
	auto alpha = ImGui::GetStyle().Alpha;

	builder->Input(node->inputs[0].id);
	ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);
	DrawPinIcon(node->inputs[0], false, (int)(alpha * 255));
	HelpMarker2(node->inputs[0].comment);
	ImGui::Spring(0);
	ImGui::PopStyleVar();
	builder->EndInput();
		
	for (auto& output : node->outputs)
	{
		builder->Output(output.id);
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);
		DrawPinIcon(output, false, (int)(alpha * 255));
		HelpMarker2(output.comment);
		ImGui::PopStyleVar();
		builder->EndOutput();
	}
		
	ImGui::NewLine();
	ImGui::Spring(0);
	if (ImGui::Button("+"))
	{
		facade->sendNotification(COMMANDTYPE::ADDPIN, (void*)node->id.Get());
	}
	
	builder->End();	
}


void seqeuemediator::onRegister()
{
	
}

void seqeuemediator::onRemove()
{
	
}