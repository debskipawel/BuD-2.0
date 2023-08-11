#include "PropertiesGuiLayer.h"

#include <BuD.h>
#include <imgui.h>

#include "ObjectGuiDrawerVisitor.h"

PropertiesGuiLayer::PropertiesGuiLayer(MainDataLayer& dataLayer)
	: BaseGuiLayer(dataLayer)
{
}

void PropertiesGuiLayer::DrawGui()
{
	if (ImGui::Begin("Properties"))
	{
		auto& composite = m_MainDataLayer.m_SceneDataLayer.m_SceneCAD.m_SelectedGroup;

		switch (composite.m_Objects.size())
		{
		case 0:
			break;
		case 1:
			DrawGuiForSingularObject();
			break;
		default:
			DrawGuiForComposite();
			break;
		}

		ImGui::End();
	}
}

void PropertiesGuiLayer::DrawGuiForSingularObject()
{
	std::unique_ptr<AbstractVisitor> visitor = std::make_unique<ObjectGuiDrawerVisitor>(m_MainDataLayer.m_SceneDataLayer);
	auto& composite = m_MainDataLayer.m_SceneDataLayer.m_SceneCAD.m_SelectedGroup;

	auto& [key, object] = *composite.m_Objects.begin();

	auto objectShared = object.lock();

	visitor->Visit(*objectShared);
}

void PropertiesGuiLayer::DrawGuiForComposite()
{
	auto& composite = m_MainDataLayer.m_SceneDataLayer.m_SceneCAD.m_SelectedGroup;

	auto max = ImGui::GetWindowContentRegionMax();
	auto min = ImGui::GetWindowContentRegionMin();

	auto cursorPos = ImGui::GetCursorPos();
	auto style = ImGui::GetStyle();

	auto buttonHeight = 20 + style.ItemInnerSpacing.y;
	auto fullHeight = 2 * buttonHeight + 2 * style.ItemInnerSpacing.y;

	if (max.y - fullHeight > cursorPos.y)
	{
		ImGui::SetCursorPos({ min.x, max.y - fullHeight });
	}

	if (ImGui::Button("Apply transform", ImVec2(max.x - min.x, buttonHeight)))
	{
		// zero the transform and apply
	}

	ImGui::Separator();

	if (ImGui::Button("Delete all selected", ImVec2(max.x - min.x, buttonHeight)))
	{
		m_MainDataLayer.m_AppStateDataLayer.Freeze();
		ImGui::OpenPopup("Delete selected ###delete_selected_popup");
	}

	if (ImGui::BeginPopupModal("Delete all selected? ###delete_selected_popup", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize))
	{
		auto max = ImGui::GetWindowContentRegionMax();
		auto min = ImGui::GetWindowContentRegionMin();

		if (ImGui::Button("Yes###delete_yes_button", { 150, 0 }))
		{
			auto& scene = m_MainDataLayer.m_SceneDataLayer.m_SceneCAD;
			scene.DeleteSelected();
			ImGui::CloseCurrentPopup();
			m_MainDataLayer.m_AppStateDataLayer.Unfreeze();
		}

		if (ImGui::Button("No###delete_no_button", { 150, 0 }))
		{
			ImGui::CloseCurrentPopup();
			m_MainDataLayer.m_AppStateDataLayer.Unfreeze();
		}

		ImGui::EndPopup();
	}
}
