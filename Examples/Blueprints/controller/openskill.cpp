#include "openskill.h"
#include "define.h"
#include "model/language.h"
#include "model/settingproxy.h"
#include <algorithm>
#include <Windows.h>

void openskill::execute(PureMVC::INotification* note)
{
	PureMVC::IFacade *facade = this->getFacade();
	Node* node = static_cast<Node*>(note->getBody());
	
	if (node && node->skillid > 0 && node->saved == true)
	{
		settingproxy* setting = dynamic_cast<settingproxy*>(facade->retrieveProxy(settingproxy::NAME));
		assert(setting != nullptr);

		std::string filepath = setting->notepad() + " " + setting->workspace() + "\\" + std::to_string(node->skillid) + ".json";
		STARTUPINFO INFO;
		memset(&INFO, 0, sizeof(INFO));
		INFO.cb = sizeof(STARTUPINFO);

		PROCESS_INFORMATION PINFO;
		memset(&PINFO, 0, sizeof(PROCESS_INFORMATION));
		if (::CreateProcessA(NULL, &filepath[0], NULL, NULL, FALSE, 0, NULL, NULL, &INFO, &PINFO))
		{
			::CloseHandle(PINFO.hProcess);
			::CloseHandle(PINFO.hThread);
		}
	}
}