#include "mainmediator.h"
#include "define.h"
#include "imgui_node_editor.h"
#include "model/language.h"
#include "model/mainboardproxy.h"
#include "Application.h"
#include <ax/Builders.h>
#include "imgui_internal.h"
const std::string mainmediator::NAME = "mainmediator";

namespace util = ax::NodeEditor::Utilities;
const int            s_PinIconSize = 24;
ImTextureID          s_HeaderBackground = nullptr;
//static ImTextureID          s_SampleImage = nullptr;
ImTextureID          s_SaveIcon = nullptr;
ImTextureID          s_RestoreIcon = nullptr;

extern const char* arrar_obj_edit_name;

mainmediator::mainmediator()
	:PureMVC::Mediator(mainmediator::NAME)
{
	editobj_.type = 0;
	editobj_.pinid = 0;
	editobj_.opid = 0;
}

std::vector<int> mainmediator::listNotificationInterests()
{
	return std::vector<int>{COMMANDTYPE::DISPLAYMAIN, COMMANDTYPE::SHOWOBJEDIT};
}

void mainmediator::handleNotification(PureMVC::INotification* notification)
{
	int name = notification->getName();
	int type = notification->getType();
	void* body = notification->getBody();
	PureMVC::IFacade *facade = this->getFacade();
	if (name == COMMANDTYPE::DISPLAYMAIN)
	{
		mainboardproxy* maindata = dynamic_cast<mainboardproxy*>(
			facade->retrieveProxy(mainboardproxy::NAME));
		assert(maindata);
		ImGui::SameLine(0.0f, 12.0f);
		ed::Begin("Node editor");
		{
			auto cursorTopLeft = ImGui::GetCursorScreenPos();
			util::BlueprintNodeBuilder builder(s_HeaderBackground, Application_GetTextureWidth(s_HeaderBackground), Application_GetTextureHeight(s_HeaderBackground));
						
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

				facade->sendNotification(COMMANDTYPE::DISPLAYDELETE);
			}

			ImGui::SetCursorScreenPos(cursorTopLeft);
		}

		auto openPopupPosition = ImGui::GetMousePos();
		ed::Suspend();
		if (ed::ShowBackgroundContextMenu())
		{
			ImGui::OpenPopup("mainmenu");
		}
		else if (this->editobj_.type != 0)
		{
			auto pin = maindata->GetPin(this->editobj_.pinid);
			if (pin == nullptr)
			{
				editobj_.type = 0;
				editobj_.pinid = 0;
				editobj_.opid = 0;
			}
			else
			{
				if (this->editobj_.type == 1)
				{
					ImGuiWindowFlags flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoSavedSettings;
					if (ImGui::BeginPopupEx(this->editobj_.opid, flags))
					{
						//ImGui::PushItemWidth(360);
						ImGui::Text("         Input Json Object Format         ");
						//ImGui::PopItemWidth();
						ImGui::Separator();
						ImGui::InputTextMultiline("##1", pin->value.str_, IM_ARRAYSIZE(pin->value.str_), ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 16));
						ImGui::EndPopup();
					}
					else
					{
						editobj_.type = 0;
						editobj_.pinid = 0;
						editobj_.opid = 0;
					}
				}
			}
		}
		ed::Resume();

		if (ImGui::IsPopupOpen("mainmenu"))
		{
			facade->sendNotification(COMMANDTYPE::MAINMENU);
		}

		ed::End();
	}
	else {
		EditObj* pair = static_cast<EditObj*>(body);
		editobj_ = *pair;
	}
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