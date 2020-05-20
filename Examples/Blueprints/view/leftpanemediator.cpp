#include "leftpanemediator.h"
#include "define.h"
#include "imgui_node_editor.h"

const std::string leftpanemediator::NAME = "leftpanemediator"
;
leftpanemediator::leftpanemediator() :
	PureMVC::Mediator(leftpanemediator::NAME)
{
	showstyle_ = nullptr;
}

std::vector<int> leftpanemediator::listNotificationInterests()
{
	return std::vector<int>{COMMANDTYPE::DISPLAYLEFTPANE, COMMANDTYPE::SHOWSTYPE};
}

void leftpanemediator::handleNotification(PureMVC::INotification* notification)
{
	int name = notification->getName();
	int type = notification->getType();
	void* body = notification->getBody();
	if (name == COMMANDTYPE::DISPLAYLEFTPANE)
	{
		PureMVC::IFacade* facade = this->getFacade();
		void* body = notification->getBody();
		float paneWidth = *(float*)body;
		auto& io = ImGui::GetIO();

		ImGui::BeginChild("Selection", ImVec2(paneWidth, 0));

		paneWidth = ImGui::GetContentRegionAvailWidth();

		//static bool showStyleEditor = false;
		ImGui::BeginHorizontal("Style Editor", ImVec2(paneWidth, 0));
		ImGui::Spring(0.0f, 0.0f);
		if (ImGui::Button("Zoom to Content"))
			ed::NavigateToContent();
		ImGui::Spring(0.0f);

		ImGui::Spring();
		if (ImGui::Button("Edit Style"))
		{
			facade->sendNotification(COMMANDTYPE::OPENSTYLE);
		}

		if (this->showstyle_ != nullptr && !(*this->showstyle_))
		{
			this->showstyle_ = nullptr;
			facade->sendNotification(COMMANDTYPE::CLOSESTYLE);
		}

		ImGui::EndHorizontal();
		ImGui::EndChild();
	}
	else if (name == COMMANDTYPE::SHOWSTYPE)
	{
		this->showstyle_ = (bool*)body;
	}
	
}

void leftpanemediator::onRegister()
{

}

void leftpanemediator::onRemove()
{

}
