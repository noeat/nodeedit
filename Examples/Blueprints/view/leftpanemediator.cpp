#include "leftpanemediator.h"
#include "define.h"
#include "imgui_node_editor.h"
#include "Application.h"
#include "model/mainboardproxy.h"
#include "model/language.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui_internal.h>
#include "common.h"
#include "utiliti.h"
#include <algorithm>
#include <list>
#include <set>

static bool Splitter(bool split_vertically, float thickness, float* size1, float* size2, float min_size1, float min_size2, float splitter_long_axis_size = -1.0f)
{
	using namespace ImGui;
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = g.CurrentWindow;
	ImGuiID id = window->GetID("##Splitter");
	ImRect bb;
	bb.Min = window->DC.CursorPos + (split_vertically ? ImVec2(*size1, 0.0f) : ImVec2(0.0f, *size1));
	bb.Max = bb.Min + CalcItemSize(split_vertically ? ImVec2(thickness, splitter_long_axis_size) : ImVec2(splitter_long_axis_size, thickness), 0.0f, 0.0f);
	return SplitterBehavior(bb, id, split_vertically ? ImGuiAxis_X : ImGuiAxis_Y, size1, size2, min_size1, min_size2, 0.0f);
}

extern const int            s_PinIconSize;
extern ImTextureID          s_HeaderBackground;
extern ImTextureID          s_SaveIcon;
extern ImTextureID          s_RestoreIcon;

const std::string leftpanemediator::NAME = "leftpanemediator"
;
leftpanemediator::leftpanemediator() :
	PureMVC::Mediator(leftpanemediator::NAME)
{
	
}

std::vector<int> leftpanemediator::listNotificationInterests()
{
	return std::vector<int>{COMMANDTYPE::DISPLAYLEFTPANE};
}

void leftpanemediator::handleNotification(PureMVC::INotification* notification)
{	
	static float leftPaneWidth = 400.0f;
	static float rightPaneWidth = 800.0f;
	Splitter(true, 4.0f, &leftPaneWidth, &rightPaneWidth, 50.0f, 50.0f);
	float leftpane = leftPaneWidth - 4.0;
	PureMVC::IFacade* facade = this->getFacade();
	ImGui::BeginChild("Selection", ImVec2(leftpane, 0));
	leftpane = ImGui::GetContentRegionAvailWidth();
	//static bool showStyleEditor = false;
	ImGui::BeginHorizontal("Style Editor", ImVec2(leftpane, 0));
	ImGui::Spring(0.0f, 0.0f);
	if (ImGui::Button("Zoom to Content"))
		ed::NavigateToContent();
	
	std::vector<ed::NodeId> selectedNodes;
	std::vector<ed::LinkId> selectedLinks;
	selectedNodes.resize(ed::GetSelectedObjectCount());
	selectedLinks.resize(ed::GetSelectedObjectCount());

	int nodeCount = ed::GetSelectedNodes(selectedNodes.data(), static_cast<int>(selectedNodes.size()));
	int linkCount = ed::GetSelectedLinks(selectedLinks.data(), static_cast<int>(selectedLinks.size()));

	selectedNodes.resize(nodeCount);
	selectedLinks.resize(linkCount);

	ImGui::Spring(0.0f);
	if (ImGui::Button("Show Flow"))
	{
		facade->sendNotification(COMMANDTYPE::SHOWFLOW, &selectedNodes);
	}

	ImGui::Spring();
	if (ImGui::Button("Edit Style"))
	{
		facade->sendNotification(COMMANDTYPE::OPENSTYLE);
	}
	
	ImGui::EndHorizontal();
	
	auto& io = ImGui::GetIO();
	mainboardproxy* proxy = dynamic_cast<mainboardproxy*>(
		facade->retrieveProxy(mainboardproxy::NAME));
	assert(proxy);

	int saveIconWidth = Application_GetTextureWidth(s_SaveIcon);
	int saveIconHeight = Application_GetTextureWidth(s_SaveIcon);
	int restoreIconWidth = Application_GetTextureWidth(s_RestoreIcon);
	int restoreIconHeight = Application_GetTextureWidth(s_RestoreIcon);

	ImGui::GetWindowDrawList()->AddRectFilled(
		ImGui::GetCursorScreenPos(),
		ImGui::GetCursorScreenPos() + ImVec2(leftpane, ImGui::GetTextLineHeight()),
		ImColor(ImGui::GetStyle().Colors[ImGuiCol_HeaderActive]), ImGui::GetTextLineHeight() * 0.25f);
	ImGui::Spacing(); ImGui::SameLine();
	ImGui::TextUnformatted("SKills");
	ImGui::Indent();
	for (auto& node : proxy->skills())
	{
		ImGui::PushID(node->id.AsPointer());
		auto start = ImGui::GetCursorScreenPos();

		/*if (const auto progress = GetTouchProgress(node.ID))
		{
			ImGui::GetWindowDrawList()->AddLine(
				start + ImVec2(-8, 0),
				start + ImVec2(-8, ImGui::GetTextLineHeight()),
				IM_COL32(255, 0, 0, 255 - (int)(255 * progress)), 4.0f);
		}*/

		bool isSelected = std::find(selectedNodes.begin(), selectedNodes.end(), node->id) != selectedNodes.end();
		if (ImGui::Selectable((std::string(node->name) + "##" + std::to_string(reinterpret_cast<uintptr_t>(node->id.AsPointer()))).c_str(), &isSelected))
		{
			if (io.KeyCtrl)
			{
				if (isSelected)
					ed::SelectNode(node->id, true);
				else
					ed::DeselectNode(node->id);
			}
			else
				ed::SelectNode(node->id, false);

			ed::NavigateToSelection();
		}
	/*	if (ImGui::IsItemHovered() && !node->State.empty())
			ImGui::SetTooltip("State: %s", node.State.c_str());*/

		auto id = std::string("(") + std::to_string(node->skillid) + ")";
		auto textSize = ImGui::CalcTextSize(id.c_str(), nullptr);
		auto iconPanelPos = start + ImVec2(
			leftpane - ImGui::GetStyle().FramePadding.x - ImGui::GetStyle().IndentSpacing - saveIconWidth - restoreIconWidth - ImGui::GetStyle().ItemInnerSpacing.x * 1,
			(ImGui::GetTextLineHeight() - saveIconHeight) / 2);
		ImGui::GetWindowDrawList()->AddText(
			ImVec2(iconPanelPos.x - textSize.x - ImGui::GetStyle().ItemInnerSpacing.x, start.y),
			IM_COL32(255, 255, 255, 255), id.c_str(), nullptr);

		auto drawList = ImGui::GetWindowDrawList();
		ImGui::SetCursorScreenPos(iconPanelPos);
		ImGui::SetItemAllowOverlap();
		if (!node->saved && node->skillid > 0)
		{
			if (ImGui::InvisibleButton("save", ImVec2((float)saveIconWidth, (float)saveIconHeight)))
			{
				node->saved = true;
				facade->sendNotification(COMMANDTYPE::SAVESKILL, node);
			}				

			if (ImGui::IsItemActive())
				drawList->AddImage(s_SaveIcon, ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), ImVec2(0, 0), ImVec2(1, 1), IM_COL32(255, 255, 255, 96));
			else if (ImGui::IsItemHovered())
				drawList->AddImage(s_SaveIcon, ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), ImVec2(0, 0), ImVec2(1, 1), IM_COL32(255, 255, 255, 255));
			else
				drawList->AddImage(s_SaveIcon, ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), ImVec2(0, 0), ImVec2(1, 1), IM_COL32(255, 255, 255, 160));
		}
		else
		{
			ImGui::Dummy(ImVec2((float)saveIconWidth, (float)saveIconHeight));
			drawList->AddImage(s_SaveIcon, ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), ImVec2(0, 0), ImVec2(1, 1), IM_COL32(255, 255, 255, 32));
		}

		ImGui::SameLine(0, ImGui::GetStyle().ItemInnerSpacing.x);
		ImGui::SetItemAllowOverlap();
		if (node->saved && node->skillid > 0)
		{
			if (ImGui::InvisibleButton("restore", ImVec2((float)restoreIconWidth, (float)restoreIconHeight)))
			{
				facade->sendNotification(COMMANDTYPE::OPENSKILL, node);
			}

			if (ImGui::IsItemActive())
				drawList->AddImage(s_RestoreIcon, ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), ImVec2(0, 0), ImVec2(1, 1), IM_COL32(255, 255, 255, 96));
			else if (ImGui::IsItemHovered())
				drawList->AddImage(s_RestoreIcon, ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), ImVec2(0, 0), ImVec2(1, 1), IM_COL32(255, 255, 255, 255));
			else
				drawList->AddImage(s_RestoreIcon, ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), ImVec2(0, 0), ImVec2(1, 1), IM_COL32(255, 255, 255, 160));
		}
		else
		{
			ImGui::Dummy(ImVec2((float)restoreIconWidth, (float)restoreIconHeight));
			drawList->AddImage(s_RestoreIcon, ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), ImVec2(0, 0), ImVec2(1, 1), IM_COL32(255, 255, 255, 32));
		}

		ImGui::SameLine(0, 0);
		ImGui::SetItemAllowOverlap();
		ImGui::Dummy(ImVec2(0, (float)restoreIconHeight));

		ImGui::PopID();
	}
	ImGui::Unindent();


	ImGui::GetWindowDrawList()->AddRectFilled(
		ImGui::GetCursorScreenPos(),
		ImGui::GetCursorScreenPos() + ImVec2(leftpane, ImGui::GetTextLineHeight()),
		ImColor(ImGui::GetStyle().Colors[ImGuiCol_HeaderActive]), ImGui::GetTextLineHeight() * 0.25f);
	ImGui::Spacing(); ImGui::SameLine();
	ImGui::TextUnformatted("Selection Nodes");
	ImGui::BeginHorizontal("Selection Stats", ImVec2(leftpane, 0));

	ImGui::Spring();
	if (ImGui::Button("Deselect All"))
		ed::ClearSelection();
	ImGui::EndHorizontal();
	ImGui::Indent();

	languageproxy* language = dynamic_cast<languageproxy*>(
		facade->retrieveProxy(languageproxy::NAME));
	assert(language);
	using nodetype_t = decltype(*selectedNodes.begin());
	std::sort(selectedNodes.begin(), selectedNodes.end(), [](nodetype_t& l, nodetype_t& r) 
	{
		return l.Get() < r.Get();
	});

	for (int i = 0; i < nodeCount; ++i) 
	{
		auto nodeid = selectedNodes[i].Get();
		auto node = proxy->getnode(nodeid);
		ImGui::GetWindowDrawList()->AddRectFilled(
			ImGui::GetCursorScreenPos(),
			ImGui::GetCursorScreenPos() + ImVec2(leftpane, ImGui::GetTextLineHeight()),
			ImColor(ImGui::GetStyle().Colors[ImGuiCol_Tab]), ImGui::GetTextLineHeight() * 0.25f);
		ImGui::Spacing(); ImGui::SameLine();
		std::string name(node->name, ImGui::FindRenderedTextEnd(node->name) - node->name);
		char buff[128];
		snprintf(buff, 128, "Node:%s (%d)", name.c_str(), nodeid);
		if (node->type == NODETYPE::ENTRY)
		{
			if (ImGui::TreeNode(buff))
			{
				ImGui::Columns(2, "property");
				ImGui::Separator();
				ImGui::TextWrapped(language->getstr("skillname"));
				ImGui::NextColumn();
				if (ImGui::DragInt("##skillid", &node->skillid, 1, 0, 10000000, "%d"))
				{
					int skillid = node->skillid;
					node->saved = false;
					std::list<Node*> open;
					std::set<Node*> close;
					open.push_back(node);
					while (!open.empty())
					{
						auto n = open.front();
						n->skillid = skillid;
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
				}
				ImGui::Columns(1);
				ImGui::Separator();
				ImGui::TreePop();
			}
		}
		else
		{
			if (ImGui::TreeNode(buff))
			{
				assert(node->inputs.size() > 0);
				ImGui::Columns(2, "property");
				ImGui::Separator();
				ImGui::TextWrapped(language->getstr("enterflow"));
				ImGui::NextColumn();
				if (node->inputs[0].links.size() > 0)
				{
					auto lastnode = node->inputs[0].links[0]->node;
					std::string name(lastnode->name, ImGui::FindRenderedTextEnd(lastnode->name) - lastnode->name);
					char buff[128];
					snprintf(buff, 128, "-> Node:%s (%d)", name.c_str(), lastnode->id.Get());
					ImGui::TextWrapped(buff);
				}
				else
				{
					ImGui::TextWrapped("");
				}				
				
				ImGui::Columns(1);
				for (int i = 1; i < node->inputs.size(); ++i)
				{
					auto& pin = node->inputs[i];
					ImGui::Columns(2, "property");
					ImGui::Separator();
					std::string name(pin.name, ImGui::FindRenderedTextEnd(pin.name) - pin.name);
					ImGui::TextWrapped(name.c_str());
					ImGui::NextColumn();
					if (pin.links.size() > 0)
					{
						auto lastnode = pin.links[0]->node;
						std::string name(lastnode->name, ImGui::FindRenderedTextEnd(lastnode->name) - lastnode->name);
						char buff[128];
						snprintf(buff, 128, "-> Node:%s (%d)", name.c_str(), lastnode->id.Get());
						ImGui::TextWrapped(buff);
					}
					else
					{
						if (pin.type == PinType::Bool)
						{
							if (pin.value.bool_)
							{
								ImGui::TextWrapped("True");
							}
							else
							{
								ImGui::TextWrapped("False");
							}
						}
						else if (pin.type == PinType::Int)
						{
							ImGui::TextWrapped(std::to_string(pin.value.int_).c_str());
						}
						else if (pin.type == PinType::Float)
						{
							ImGui::TextWrapped(std::to_string(pin.value.float_).c_str());
						}
						else if (pin.type == PinType::String)
						{
							ImGui::TextWrapped(pin.value.str_);
						}
						else if (pin.type == PinType::Object)
						{
							tokenizer tok(pin.name, '#');
							if (tok.size() > 2 && atoi(tok[2]) == 0)
							{
								ImGui::TextWrapped(language->getstr("instowner"));
							}
							else
							{
								ImGui::TextWrapped(pin.value.str_);
							}
						}
					}
					ImGui::Columns(1);
				}
/*
				for (int i = 1; i < 10; ++i)
				{
					ImGui::Columns(2, "property");
					ImGui::Separator();
					ImGui::TextWrapped("The quick brown fox jumps over the lazy dog.");
					ImGui::TextWrapped("Hello Left");
					ImGui::NextColumn();
					ImGui::TextWrapped("The quick brown fox jumps over the lazy dog.");
					ImGui::TextWrapped("Hello Right");
					ImGui::Columns(1);
					ImGui::Separator();
				}
*/ImGui::Separator();
				ImGui::TreePop();
			}
		}	
	} 
	
	ImGui::Unindent();

	

	ImGui::EndChild();
}

void leftpanemediator::onRegister()
{

}

void leftpanemediator::onRemove()
{

}
