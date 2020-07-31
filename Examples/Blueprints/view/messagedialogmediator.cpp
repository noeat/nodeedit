#include "messagedialogmediator.h"
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
const std::string messagedialogmediator::NAME = "messagedialogmediator";
messagedialogmediator::messagedialogmediator()
	:PureMVC::Mediator(messagedialogmediator::NAME)
{
	
}

std::vector<int> messagedialogmediator::listNotificationInterests()
{
	return std::vector<int>{COMMANDTYPE::DISPLAYLEFTPANE};
}

void messagedialogmediator::handleNotification(PureMVC::INotification* notification)
{
	ImGui::OpenPopup(this->title_.c_str());
	ImGui::SetNextWindowSize(ImVec2(400, 0));
	if (ImGui::BeginPopupModal(this->title_.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::NewLine();
		ImGui::TextWrapped(this->message_.c_str());
		ImGui::Separator();		

		ImGui::NewLine();
		ImGui::SameLine(140);
		if (ImGui::Button("Close", ImVec2(120, 0))) { 
			ImGui::CloseCurrentPopup(); 
			delete this->getFacade()->removeMediator(messagedialogmediator::NAME);
		}
		ImGui::SameLine(260, 140);
		ImGui::EndPopup();
	}	
}


void messagedialogmediator::onRegister()
{
	
}

void messagedialogmediator::onRemove()
{
	
}