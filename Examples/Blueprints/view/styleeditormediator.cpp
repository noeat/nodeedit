#include "styleeditormediator.h"
#include "define.h"
#include "imgui_node_editor.h"

const std::string styleeditormediator::NAME = "styleeditormediator";

styleeditormediator::styleeditormediator()
	:PureMVC::Mediator(styleeditormediator::NAME)
{

}

std::vector<int> styleeditormediator::listNotificationInterests()
{
	return std::vector<int>{COMMANDTYPE::DISPLAYLEFTPANE, COMMANDTYPE::SHOWSTYPE};
}

void styleeditormediator::handleNotification(PureMVC::INotification* notification)
{
	int name = notification->getName();
	int type = notification->getType();
	void* body = notification->getBody();
	if (name == COMMANDTYPE::DISPLAYLEFTPANE)
	{
		show();
	}
	else if (name == COMMANDTYPE::SHOWSTYPE)
	{
		this->showstyle_ = (bool*)body;
	}
}

void styleeditormediator::show()
{
	PureMVC::IFacade* facade = this->getFacade();
	if (!ImGui::Begin("Style", this->showstyle_))
	{
		facade->sendNotification(COMMANDTYPE::CLOSESTYLE);
		ImGui::End();
		return;
	}

	auto paneWidth = ImGui::GetContentRegionAvailWidth();

	auto& editorStyle = ed::GetStyle();
	ImGui::BeginHorizontal("Style buttons", ImVec2(paneWidth, 0), 1.0f);
	ImGui::TextUnformatted("Values");
	ImGui::Spring();
	if (ImGui::Button("Reset to defaults"))
		editorStyle = ed::Style();
	ImGui::EndHorizontal();
	ImGui::Spacing();
	ImGui::DragFloat4("Node Padding", &editorStyle.NodePadding.x, 0.1f, 0.0f, 40.0f);
	ImGui::DragFloat("Node Rounding", &editorStyle.NodeRounding, 0.1f, 0.0f, 40.0f);
	ImGui::DragFloat("Node Border Width", &editorStyle.NodeBorderWidth, 0.1f, 0.0f, 15.0f);
	ImGui::DragFloat("Hovered Node Border Width", &editorStyle.HoveredNodeBorderWidth, 0.1f, 0.0f, 15.0f);
	ImGui::DragFloat("Selected Node Border Width", &editorStyle.SelectedNodeBorderWidth, 0.1f, 0.0f, 15.0f);
	ImGui::DragFloat("Pin Rounding", &editorStyle.PinRounding, 0.1f, 0.0f, 40.0f);
	ImGui::DragFloat("Pin Border Width", &editorStyle.PinBorderWidth, 0.1f, 0.0f, 15.0f);
	ImGui::DragFloat("Link Strength", &editorStyle.LinkStrength, 1.0f, 0.0f, 500.0f);
	ImGui::DragFloat("Scroll Duration", &editorStyle.ScrollDuration, 0.001f, 0.0f, 2.0f);
	ImGui::DragFloat("Flow Marker Distance", &editorStyle.FlowMarkerDistance, 1.0f, 1.0f, 200.0f);
	ImGui::DragFloat("Flow Speed", &editorStyle.FlowSpeed, 1.0f, 1.0f, 2000.0f);
	ImGui::DragFloat("Flow Duration", &editorStyle.FlowDuration, 0.001f, 0.0f, 5.0f);
	ImGui::DragFloat("Group Rounding", &editorStyle.GroupRounding, 0.1f, 0.0f, 40.0f);
	ImGui::DragFloat("Group Border Width", &editorStyle.GroupBorderWidth, 0.1f, 0.0f, 15.0f);
	ImGui::Separator();
	static ImGuiColorEditFlags edit_mode = ImGuiColorEditFlags_RGB;
	ImGui::BeginHorizontal("Color Mode", ImVec2(paneWidth, 0), 1.0f);
	ImGui::TextUnformatted("Filter Colors");
	ImGui::Spring();
	ImGui::RadioButton("RGB", &edit_mode, ImGuiColorEditFlags_RGB);
	ImGui::Spring(0);
	ImGui::RadioButton("HSV", &edit_mode, ImGuiColorEditFlags_HSV);
	ImGui::Spring(0);
	ImGui::RadioButton("HEX", &edit_mode, ImGuiColorEditFlags_HEX);
	ImGui::EndHorizontal();

	static ImGuiTextFilter filter;
	filter.Draw("", paneWidth);

	ImGui::Spacing();

	ImGui::PushItemWidth(-160);
	for (int i = 0; i < ed::StyleColor_Count; ++i)
	{
		auto name = ed::GetStyleColorName((ed::StyleColor)i);
		if (!filter.PassFilter(name))
			continue;

		ImGui::ColorEdit4(name, &editorStyle.Colors[i].x, edit_mode);
	}
	ImGui::PopItemWidth();

	ImGui::End();
}

void styleeditormediator::onRegister()
{
	this->showstyle_ = nullptr;
}

void styleeditormediator::onRemove() 
{
	this->showstyle_ = nullptr;
}