#include "mainmediator.h"
#include "define.h"
#include "imgui_node_editor.h"
#include "model/language.h"
#include "model/mainboardproxy.h"
#include "Application.h"
#include <ax/Builders.h>

const std::string mainmediator::NAME = "mainmediator";

namespace util = ax::NodeEditor::Utilities;
static const int            s_PinIconSize = 24;
static ImTextureID          s_HeaderBackground = nullptr;
//static ImTextureID          s_SampleImage = nullptr;
static ImTextureID          s_SaveIcon = nullptr;
static ImTextureID          s_RestoreIcon = nullptr;



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
		util::BlueprintNodeBuilder builder(s_HeaderBackground, Application_GetTextureWidth(s_HeaderBackground), Application_GetTextureHeight(s_HeaderBackground));

		mainboardproxy* maindata = dynamic_cast<mainboardproxy*>(
			facade->retrieveProxy(mainboardproxy::NAME));
		assert(maindata);
		for (auto iter = maindata->nodes().begin(); iter != maindata->nodes().end(); ++iter)
		{
			if (iter->second->show)
			{
				auto item = std::make_pair(&builder, iter->second);
				facade->sendNotification(COMMANDTYPE::DISPLAYNODE + iter->first, &item);
			}
		}

		if (!maindata->nodes().empty())
		{
			/*std::pair<ed::PinId, ed::PinId> link;
			if (ed::QueryNewLink(&link.first, &link.second))
			{*/
				facade->sendNotification(COMMANDTYPE::DISPLAYLINK);
			//}
		}
		
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
	s_HeaderBackground = Application_LoadTexture("Data/BlueprintBackground.png");
	s_SaveIcon = Application_LoadTexture("Data/ic_save_white_24dp.png");
	s_RestoreIcon = Application_LoadTexture("Data/ic_restore_white_24dp.png");
}

void mainmediator::onRemove()
{
	auto releaseTexture = [](ImTextureID& id)
	{
		if (id)
		{
			Application_DestroyTexture(id);
			id = nullptr;
		}
	};

	releaseTexture(s_RestoreIcon);
	releaseTexture(s_SaveIcon);
	releaseTexture(s_HeaderBackground);
}