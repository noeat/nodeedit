#include "actionmediator.h"
#include "define.h"
#include "imgui_node_editor.h"
#include "model/language.h"
#include "common.h"
#include <ax/Builders.h>
#include <ax/Widgets.h>
#include <ax/Drawing.h>
#include "imgui_internal.h"
#include "common.h"
#include "utiliti.h"

namespace util = ax::NodeEditor::Utilities;
static const int            s_PinIconSize = 14;
ImColor GetIconColor(PinType type);
const char* arrar_obj_edit_name = "editarray";

void DrawPinIcon(const Pin& pin, bool connected, int alpha);

actionmediator::actionmediator(int nodeid)
	:PureMVC::Mediator(id2mediatorname(nodeid)), nodeid_(nodeid)
{
	
}

std::vector<int> actionmediator::listNotificationInterests()
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
void actionmediator::handleNotification(PureMVC::INotification* notification)
{
	std::pair<util::BlueprintNodeBuilder*, Node*> *item =
		(std::pair<util::BlueprintNodeBuilder*, Node*>*)notification->getBody();
	PureMVC::IFacade* facade = this->getFacade();
	auto builder = item->first;
	auto node = item->second;
	assert(node != nullptr && node->type == NODETYPE::ACTION);
	builder->Begin(node->id);
	builder->Header(node->color);
	ImGui::Spring(0);
	HelpMarker(node->comment);
	ImGui::TextUnformatted(node->name, ImGui::FindRenderedTextEnd(node->name));
	ImGui::Dummy(ImVec2(0, 20));
	ImGui::Spring(1);

	builder->EndHeader();

	auto alpha = ImGui::GetStyle().Alpha;

	for (auto& input : node->inputs)
	{
		builder->Input(input.id);
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);
		DrawPinIcon(input, false, (int)(alpha * 255));
		HelpMarker2(input.comment);
		if (input.type == PinType::Bool)
		{
			if (input.links.empty())
			{
				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(3, 2));
				ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(3, 2));
				ImGui::Checkbox(input.name, &input.value.bool_);
				ImGui::PopStyleVar(2);
			}
			else
			{
				ImGui::TextUnformatted(input.name, ImGui::FindRenderedTextEnd(input.name));
			}
		}
		else if (input.type == PinType::Int)
		{
			if (input.links.empty())
			{
				ImGui::BeginVertical("inputint2");
				ImGui::PushItemWidth(60);
				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(3, 2));
				ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(3, 2));
				ImGui::InputInt(input.name, &input.value.int_, 1, 10);
				ImGui::PopStyleVar(2);
				ImGui::PopItemWidth();
				ImGui::EndVertical();
			}
			else
			{
				ImGui::TextUnformatted(input.name, ImGui::FindRenderedTextEnd(input.name));
			}
		}
		else if (input.type == PinType::Float)
		{
			if (input.links.empty())
			{
				ImGui::BeginVertical("inputint");
				ImGui::PushItemWidth(45);
				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(3, 2));
				ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(3, 2));
				ImGui::DragFloat(input.name, &input.value.float_, 0.03333, -1, 3600);
				ImGui::PopStyleVar(2);
				ImGui::PopItemWidth();
				ImGui::EndVertical();
			}
			else
			{
				ImGui::TextUnformatted(input.name, ImGui::FindRenderedTextEnd(input.name));
			}
		}
		else if (input.type == PinType::String)
		{
			if (input.links.empty())
			{
				ImGui::PushItemWidth(80);
				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(3, 2));
				ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(3, 2));
				ImGui::InputText(input.name, input.value.str_, sizeof(input.value.str_));
				ImGui::PopStyleVar(2);
				ImGui::PopItemWidth();
			}
			else
			{
				ImGui::TextUnformatted(input.name, ImGui::FindRenderedTextEnd(input.name));
			}
		}		
		else
		{
			//ImGui::TextUnformatted(input.name, ImGui::FindRenderedTextEnd(input.name));
			int len = strlen(input.name);
			const char* end = ImGui::FindRenderedTextEnd(input.name) + 2;
			if (end >= input.name + len)
			{
				ImGui::TextUnformatted(input.name, end-2);
			}
			else
			{
				int type = atoi(end);
				if (type == 0)
				{
					ImGui::TextUnformatted(input.name, end - 2);
				}
				else if (type == 1)
				{
					if (ImGui::Button(input.name))
					{
						ed::Suspend();
						//ImGui::OpenPopup(arrar_obj_edit_name);
						ImGuiContext& g = *GImGui;
						auto id = g.CurrentWindow->GetID(arrar_obj_edit_name);
						ImGui::OpenPopupEx(id);
						ed::Resume();
						EditObj pair;
						pair.type = 1;
						pair.pinid = input.id.Get();
						pair.opid = id;// (1, input.id.Get(), id);
						facade->sendNotification(COMMANDTYPE::SHOWOBJEDIT, &pair);
					}					
				}
				else if (type == 2)
				{

				}
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


void actionmediator::onRegister()
{
	
}

void actionmediator::onRemove()
{
	
}