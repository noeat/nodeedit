#include "workspacemediator.h"
#include "define.h"
#include "imgui_node_editor.h"
#include "model/language.h"
#include <shlobj.h>
#include <filesystem>
const std::string workspacemediator::NAME = "workspacemediator";

workspacemediator::workspacemediator()
	:PureMVC::Mediator(workspacemediator::NAME)
{

}

std::vector<int> workspacemediator::listNotificationInterests()
{
	return std::vector<int>{COMMANDTYPE::DISPLAYLEFTPANE, COMMANDTYPE::SETTING};
}

void workspacemediator::handleNotification(PureMVC::INotification* notification)
{
	int name = notification->getName();
	int type = notification->getType();
	void* body = notification->getBody();
	if (name == COMMANDTYPE::DISPLAYLEFTPANE)
	{
		PureMVC::IFacade* facade = this->getFacade();
		languageproxy* language = dynamic_cast<languageproxy*>(facade->retrieveProxy(languageproxy::NAME));
		ImGui::OpenPopup("workspace");
		if (ImGui::BeginPopupModal("workspace", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			
			ImGui::NewLine();
			ImGui::SameLine(0, 120);
			ImGui::Text(language->getstr("workspace"));
			ImGui::SameLine(0, 120);
			ImGui::NewLine();
			ImGui::Separator();

			ImGui::NewLine();
			ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * 0.9f);
			ImGui::InputText("", buff_, 260);
			ImGui::SameLine();
			if (ImGui::Button("..."))
			{
				BROWSEINFO  bi;
				bi.hwndOwner = NULL;
				bi.pidlRoot =  NULL;
				bi.pszDisplayName = NULL;
				bi.lpszTitle = NULL;
				bi.ulFlags = BIF_DONTGOBELOWDOMAIN | BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
				bi.lpfn = NULL;
				bi.iImage = 0;
				LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
				if (pidl != NULL)
				{
					SHGetPathFromIDList(pidl, buff_);					
				}
			}
			ImGui::NewLine();
			ImGui::SameLine(0, (ImGui::GetContentRegionAvail().x - 200) /2);
			if (ImGui::Button("OK", ImVec2(80, 16)))
			{
				std::filesystem::path p(this->buff_);
				if (!std::filesystem::exists(p))
				{
					memset(buff_, 0, sizeof(buff_));
				}
				else
				{
					facade->sendNotification(COMMANDTYPE::LAODING, (void*)this->buff_);
				}				
			}

			ImGui::SameLine(0, 40);
			if (ImGui::Button("Cancel", ImVec2(80, 16)))
			{
				exit(-1);
			}

			ImGui::EndPopup();
		}
	}
	else if (name == COMMANDTYPE::SETTING)
	{
		strncpy(buff_, (const char*)body, 260);
		std::filesystem::path p(this->buff_);
		if (!std::filesystem::exists(p))
		{
			memset(buff_, 0, sizeof(buff_));
		}
	}
}


void workspacemediator::onRegister()
{
	
}

void workspacemediator::onRemove()
{
	
}