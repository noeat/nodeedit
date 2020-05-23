#include "mainmediator.h"
#include "define.h"
#include "imgui_node_editor.h"
#include "model/language.h"
#include <shlobj.h>
#include <filesystem>
const std::string mainmediator::NAME = "mainmediator";

mainmediator::mainmediator()
	:PureMVC::Mediator(mainmediator::NAME)
{

}

std::vector<int> mainmediator::listNotificationInterests()
{
	return std::vector<int>{COMMANDTYPE::DISPLAYMAIN};
}

void mainmediator::handleNotification(PureMVC::INotification* notification)
{
	PureMVC::IFacade *facade = this->getFacade();
	ImGui::SameLine(0.0f, 12.0f);
	ed::Begin("Node editor");
	{
		auto cursorTopLeft = ImGui::GetCursorScreenPos();		

		ImGui::SetCursorScreenPos(cursorTopLeft);
	}

	auto openPopupPosition = ImGui::GetMousePos();
	ed::Suspend();
	if (ed::ShowBackgroundContextMenu())
	{
		ImGui::OpenPopup("mainmenu");		
	}
	ed::Resume();

	if (ImGui::IsPopupOpen("mainmenu"))
	{
		facade->sendNotification(COMMANDTYPE::MAINMENU);
	}
	ed::End();

}


void mainmediator::onRegister()
{
	
}

void mainmediator::onRemove()
{
	
}