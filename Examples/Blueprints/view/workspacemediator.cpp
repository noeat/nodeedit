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
	strncpy(this->notepad_, "C:\\Windows\\notepad.exe", sizeof("C:\\Windows\\notepad.exe"));
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
			if (ImGui::Button("...##1"))
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
		
			ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * 0.9f);
			ImGui::InputText("", notepad_, 260);
			ImGui::SameLine();
			if (ImGui::Button("...##2"))
			{
				char buff[260];
				::OPENFILENAMEA open;
				memset(&open, 0, sizeof(open));
				open.lStructSize = sizeof(open);
				open.lpstrFile = buff;
				open.lpstrFile[0] = 0;
				open.nMaxFile = sizeof(buff);
				open.lpstrFilter = "All(*.*)\0*.*\0Program(*.exe)\0*.exe\0";
				open.nFilterIndex = 1;
				open.Flags = OFN_PATHMUSTEXIST | OFN_READONLY;
				if (GetOpenFileNameA(&open) == TRUE)
				{
					strncpy(notepad_, buff, 260);
					::SetCurrentDirectoryA(this->workspace_);
				}
			}
			ImGui::Separator();
			ImGui::NewLine();
			ImGui::SameLine(0, (ImGui::GetContentRegionAvail().x - 200) /2);
			if (ImGui::Button("OK", ImVec2(80, 16)))
			{
				std::filesystem::path pp(this->notepad_);
				std::filesystem::path p(this->buff_);
				if (!std::filesystem::exists(p))
				{
					memset(buff_, 0, sizeof(buff_));
				}
				else if (!std::filesystem::exists(pp))
				{
					strncpy(this->notepad_, "C:\\Windows\\notepad.exe", sizeof("C:\\Windows\\notepad.exe"));
				}
				else
				{
					std::pair<const char*, const char*> pair(this->buff_, this->notepad_);
					facade->sendNotification(COMMANDTYPE::LAODING, (void*)&pair);
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
		std::pair<const char*, const char*> *pa = (std::pair<const char*, const char*>*)body;
		strncpy(buff_, (const char*)pa->first, 260);
		std::filesystem::path p(this->buff_);
		if (!std::filesystem::exists(p))
		{
			memset(buff_, 0, sizeof(buff_));
		}
		
		if (pa->second[0] != 0)
		{
			strncpy(notepad_, (const char*)pa->second, 260);
			std::filesystem::path pp(pa->second);
			if (!std::filesystem::exists(pp))
			{
				strncpy(notepad_, (const char*)pa->second, 260);
			}
		}

		::GetCurrentDirectoryA(260, this->workspace_);		
	}
}


void workspacemediator::onRegister()
{
	
}

void workspacemediator::onRemove()
{
	
}