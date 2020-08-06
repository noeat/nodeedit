#include "mainmeunmediator.h"
#include "define.h"
#include "imgui_node_editor.h"
#include "model/language.h"
#include <shlobj.h>
#include <filesystem>
const std::string mainmeunmediator::NAME = "mainmeunmediator";

mainmeunmediator::mainmeunmediator()
	:PureMVC::Mediator(mainmeunmediator::NAME)
{

}

std::vector<int> mainmeunmediator::listNotificationInterests()
{
	return std::vector<int>{COMMANDTYPE::DISPLAYMAINMENU};
}

static void HelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

void mainmeunmediator::handleNotification(PureMVC::INotification* notification)
{
	PureMVC::IFacade* facade = this->getFacade();
	const std::vector<ConfNode*>* data = (const std::vector<ConfNode*>*)notification->getBody();
	auto openPopupPosition = ImGui::GetMousePos();
	//ImGui::OpenPopup("mainmenu");
	ed::Suspend();
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));
	
	languageproxy* proxy = dynamic_cast<languageproxy*>(
		facade->retrieveProxy(languageproxy::NAME));
	assert(proxy != nullptr);

	if (ImGui::BeginPopup("mainmenu"))
	{
		int index = 0;
		int lastdiv = 0;
		for (; index < data->size(); index++)
		{
			auto item = (*data)[index];
			if (item->type >= 300)
			{
				break;
			}

			if (lastdiv != item->type / 100)
			{
				lastdiv = item->type / 100;
				ImGui::Separator();
			}

			if (ImGui::MenuItem(item->name.c_str()))
			{
				auto data = std::make_pair(item, &openPopupPosition);
				this->sendNotification(COMMANDTYPE::MAINMENUCLICK, (void*)&data);
			}
				
			if (!item->comment.empty())
			{
				ImGui::SameLine();
				HelpMarker(item->comment.c_str());
			}			
		}

		ImGui::Separator();
		std::vector<std::pair<int, int>> menus;
		if (index < data->size())
		{
			int group = (*data)[index]->type;
			int beg = index;
			for (; index < data->size(); index++)
			{
				if (group != (*data)[index]->type)
				{
					std::pair<int, int> p;
					p.first = beg;
					p.second = index;
					beg = index;
					group = (*data)[index]->type;
					menus.push_back(p);
				}
			}

			std::pair<int, int> p;
			p.first = beg;
			p.second = index ;
			menus.push_back(p);

			for (auto p : menus)
			{
				if (p.first < p.second && p.first < data->size() && p.first >= 0)
				{
					int group = (*data)[p.first]->type ;
					std::string str = std::string(std::string("actionmeun_") + std::to_string(group - 300));
					if (ImGui::BeginMenu(proxy->getstr(str.c_str())))
					{
						for (int i = p.first; i < p.second; ++i)
						{
							auto item = (*data)[i];
							if (item->type != group)
							{
								break;
							}

							if (ImGui::MenuItem(item->name.c_str()))
							{
								auto data = std::make_pair(item, &openPopupPosition);
								this->sendNotification(COMMANDTYPE::MAINMENUCLICK, (void*)&data);
							}

							if (!item->comment.empty())
							{
								ImGui::SameLine();
								HelpMarker(item->comment.c_str());
							}
						}
						ImGui::EndMenu();
					}
				}
			}
		}
		
		ImGui::EndPopup();
	}
	ImGui::PopStyleVar();
	ed::Resume();
}


void mainmeunmediator::onRegister()
{
	
}

void mainmeunmediator::onRemove()
{
	
}