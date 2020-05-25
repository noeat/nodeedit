#include "entrymediator.h"
#include "define.h"
#include "imgui_node_editor.h"
#include "model/language.h"
#include "common.h"
#include <ax/Builders.h>
namespace util = ax::NodeEditor::Utilities;

entrymediator::entrymediator(int nodeid)
	:PureMVC::Mediator(id2mediatorname(nodeid)), nodeid_(nodeid)
{
	
}

std::vector<int> entrymediator::listNotificationInterests()
{
	return std::vector<int>{COMMANDTYPE::DISPLAYNODE + this->nodeid_};
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
	ImGui::TextUnformatted(node->name);
	ImGui::Spring(1);
	ImGui::Dummy(ImVec2(0, 28));
	ImGui::Spring(0);
	builder->EndHeader();

	/*for (auto& input : node.Inputs)
	{
		auto alpha = ImGui::GetStyle().Alpha;
		if (newLinkPin && !CanCreateLink(newLinkPin, &input) && &input != newLinkPin)
			alpha = alpha * (48.0f / 255.0f);

		builder.Input(input.ID);
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);
		DrawPinIcon(input, IsPinLinked(input.ID), (int)(alpha * 255));
		ImGui::Spring(0);
		if (!input.Name.empty())
		{
			ImGui::TextUnformatted(input.Name.c_str());
			ImGui::Spring(0);
		}
		if (input.Type == PinType::Bool)
		{
			ImGui::Button("Hello");
			ImGui::Spring(0);
		}
		ImGui::PopStyleVar();
		builder.EndInput();
	}

	if (isSimple)
	{
		builder.Middle();

		ImGui::Spring(1, 0);
		ImGui::TextUnformatted(node.Name.c_str());
		ImGui::Spring(1, 0);
	}

	for (auto& output : node.Outputs)
	{
		if (!isSimple && output.Type == PinType::Delegate)
			continue;

		auto alpha = ImGui::GetStyle().Alpha;
		if (newLinkPin && !CanCreateLink(newLinkPin, &output) && &output != newLinkPin)
			alpha = alpha * (48.0f / 255.0f);

		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);
		builder.Output(output.ID);
		if (output.Type == PinType::String)
		{
			static char buffer[128] = "Edit Me\nMultiline!";
			static bool wasActive = false;

			ImGui::PushItemWidth(100.0f);
			ImGui::InputText("##edit", buffer, 127);
			ImGui::PopItemWidth();
			if (ImGui::IsItemActive() && !wasActive)
			{
				ed::EnableShortcuts(false);
				wasActive = true;
			}
			else if (!ImGui::IsItemActive() && wasActive)
			{
				ed::EnableShortcuts(true);
				wasActive = false;
			}
			ImGui::Spring(0);
		}
		if (!output.Name.empty())
		{
			ImGui::Spring(0);
			ImGui::TextUnformatted(output.Name.c_str());
		}
		ImGui::Spring(0);
		DrawPinIcon(output, IsPinLinked(output.ID), (int)(alpha * 255));
		ImGui::PopStyleVar();
		builder.EndOutput();
	}*/

	builder->End();
}


void entrymediator::onRegister()
{
	
}

void entrymediator::onRemove()
{
	
}