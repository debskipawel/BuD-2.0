#include "PropertiesGuiLayer.h"

#include <BuD.h>
#include <imgui.h>

#include <Visitors/Deletion/ObjectDeletionVisitor.h>
#include <Visitors/ObjectGui/ObjectGuiDrawerVisitor.h>

PropertiesGuiLayer::PropertiesGuiLayer(MainDataLayer& dataLayer)
	: BaseGuiLayer(dataLayer)
{
}

void PropertiesGuiLayer::DrawGui()
{
	if (ImGui::Begin("Properties"))
	{
		auto selectedCount = m_MainDataLayer.m_SceneDataLayer.m_SelectedGroup.m_SelectedObjects.size();

		switch (selectedCount)
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
	
	auto& selectedGroup = m_MainDataLayer.m_SceneDataLayer.m_SelectedGroup;
	auto& selectedId = *selectedGroup.m_SelectedObjects.begin();

	auto& object = m_MainDataLayer.m_SceneDataLayer.m_SceneCAD.m_ObjectList[selectedId];

	visitor->Visit(object);
}

void PropertiesGuiLayer::DrawGuiForComposite()
{
	auto& selectedGroup = m_MainDataLayer.m_SceneDataLayer.m_SelectedGroup;
	auto groupTransform = selectedGroup.m_GroupTransform;

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
			std::unique_ptr<AbstractVisitor> visitor = std::make_unique<ObjectDeletionVisitor>(m_MainDataLayer.m_SceneDataLayer);

			while (!m_MainDataLayer.m_SceneDataLayer.m_SelectedGroup.m_SelectedObjects.empty())
			{
				auto& id = *m_MainDataLayer.m_SceneDataLayer.m_SelectedGroup.m_SelectedObjects.begin();
				auto& object = m_MainDataLayer.m_SceneDataLayer.m_SceneCAD.m_ObjectList[id];
				
				visitor->Visit(object);
			}

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
