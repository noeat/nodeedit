#include "linkmediator.h"
#include "define.h"
#include "imgui_node_editor.h"
#include "model/language.h"
#include "model/mainboardproxy.h"
#include "Application.h"
#include <ax/Builders.h>
#include <ax/Math2D.h>
#include <set>
#include <list>

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui_internal.h>
const std::string linkmediator::NAME = "linkmediator";

namespace util = ax::NodeEditor::Utilities;
static const int            s_PinIconSize = 24;
static ImTextureID          s_HeaderBackground = nullptr;
//static ImTextureID          s_SampleImage = nullptr;
static ImTextureID          s_SaveIcon = nullptr;
static ImTextureID          s_RestoreIcon = nullptr;



linkmediator::linkmediator()
	:PureMVC::Mediator(linkmediator::NAME)
{

}

std::vector<int> linkmediator::listNotificationInterests()
{
	return std::vector<int>{COMMANDTYPE::DISPLAYLINK};
}

void linkmediator::handleNotification(PureMVC::INotification* notification)
{
	PureMVC::IFacade *facade = this->getFacade();
	std::pair<ed::PinId, ed::PinId> *item =
		(std::pair<ed::PinId, ed::PinId>*)notification->getBody();

	mainboardproxy* proxy = dynamic_cast<mainboardproxy*>(
		facade->retrieveProxy(mainboardproxy::NAME));
	assert(proxy);

	for (auto& item : proxy->nodes())
	{
		if (!item.second->outputs.empty())
		{
			for (auto opin : item.second->outputs)
			{
				for (auto pin : opin.links)
				{
					ed::LinkId linkid(pin->id.Get());
					ed::Link(linkid, opin.id, pin->id, ImColor(255, 255, 255), 2);
				}				
			}
		}
	}

	if (ed::BeginCreate(ImColor(255, 255, 255), 2.0f))
	{
		auto showLabel = [](const char* label, ImColor color)
		{
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() - ImGui::GetTextLineHeight());
			auto size = ImGui::CalcTextSize(label);

			auto padding = ImGui::GetStyle().FramePadding;
			auto spacing = ImGui::GetStyle().ItemSpacing;

			ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(spacing.x, -spacing.y));

			auto rectMin = ImGui::GetCursorScreenPos() - padding;
			auto rectMax = ImGui::GetCursorScreenPos() + size + padding;

			auto drawList = ImGui::GetWindowDrawList();
			drawList->AddRectFilled(rectMin, rectMax, color, size.y * 0.15f);
			ImGui::TextUnformatted(label);
		};

			
		ed::PinId startPinId = 0, endPinId = 0;
		if (ed::QueryNewLink(&startPinId, &endPinId))
		{
			auto startPin = proxy->GetPin(startPinId.Get());
			auto endPin = proxy->GetPin(endPinId.Get());

			if (startPin->kind == PinKind::Input)
			{
				std::swap(startPin, endPin);
				std::swap(startPinId, endPinId);
			}

			if (startPin && endPin)
			{
				if (endPin == startPin)
				{
					ed::RejectNewItem(ImColor(255, 0, 0), 2.0f);
				}
				else if (endPin->kind == startPin->kind)
				{
					showLabel("x Incompatible Pin Kind", ImColor(45, 32, 32, 180));
					ed::RejectNewItem(ImColor(255, 0, 0), 2.0f);
				}
				else if (endPin->node == startPin->node)
				{
					showLabel("x Cannot connect to self", ImColor(45, 32, 32, 180));
					ed::RejectNewItem(ImColor(255, 0, 0), 1.0f);
				}
				else if (endPin->type != startPin->type)
				{
					showLabel("x Incompatible Pin Type", ImColor(45, 32, 32, 180));
					ed::RejectNewItem(ImColor(255, 128, 128), 1.0f);
				}
				else if (!endPin->links.empty())
				{
					showLabel("x input cant links > 1", ImColor(45, 32, 32, 180));
					ed::RejectNewItem(ImColor(255, 128, 128), 1.0f);
				}
				else
				{
					showLabel("+ Create Link", ImColor(32, 45, 32, 180));
					if (ed::AcceptNewItem(ImColor(128, 255, 128), 4.0f))
					{
						startPin->links.push_back(endPin);
						endPin->links.push_back(startPin);
						
						if (startPin->node->skillid > 0)
						{
							std::list<Node*> open;
							std::set<Node*> close;
							open.push_back(endPin->node);
							close.insert(startPin->node);
							while (!open.empty())
							{
								auto n = open.front();
								n->skillid = startPin->node->skillid;
								open.pop_front();
								close.insert(n);
								for (auto& pin : n->outputs)
								{
									for (auto& link : pin.links)
									{
										if (close.count(link->node) == 0)
										{
											open.push_back(link->node);
										}
									}
								}
							}
							
							for (auto& node : proxy->skills())
							{
								if (node->skillid == startPin->node->skillid)
								{
									const_cast<Node*>(node)->saved = false;
								}							
							}
						}						
					}
				}
			}
		}		
	}
	
	ed::EndCreate();	
}


void linkmediator::onRegister()
{
	s_HeaderBackground = Application_LoadTexture("Data/BlueprintBackground.png");
	s_SaveIcon = Application_LoadTexture("Data/ic_save_white_24dp.png");
	s_RestoreIcon = Application_LoadTexture("Data/ic_restore_white_24dp.png");
}

void linkmediator::onRemove()
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