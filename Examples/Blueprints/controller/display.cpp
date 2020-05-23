#include "display.h"
#include "model/settingproxy.h"
#include "define.h"
#include "view/leftpanemediator.h"
#include "model/mainboardproxy.h"
#include "view/workspacemediator.h"
#include "model/language.h"
#include "imgui_node_editor.h"
#include "imgui.h"
void display::execute(PureMVC::INotification* note)
{		
	PureMVC::IFacade* facade = this->getFacade();
	facade->sendNotification(COMMANDTYPE::DISPLAYLEFTPANE);
	facade->sendNotification(COMMANDTYPE::CHECKSTYLECLOSE);
	facade->sendNotification(COMMANDTYPE::DISPLAYMAIN);
}