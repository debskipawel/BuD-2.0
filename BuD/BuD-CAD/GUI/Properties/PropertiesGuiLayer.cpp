#include "PropertiesGuiLayer.h"

#include <BuD.h>
#include <imgui.h>

#include <Visitors/Deletion/ObjectDeletionVisitor.h>
#include <Visitors/ObjectGui/ObjectGuiDrawerVisitor.h>
#include <Visitors/Transform/ApplyTransformVisitor.h>

PropertiesGuiLayer::PropertiesGuiLayer(MainDataLayer& dataLayer)
	: BaseGuiLayer(dataLayer)
{
}

void PropertiesGuiLayer::DrawGui()
{
	if (ImGui::Begin("Properties"))
	{
		auto selectedCount = m_MainDataLayer.m_SceneDataLayer.m_SelectedGroup.m_SelectedObjects.size();

		if (selectedCount)
		{
			DrawGuiForSelectedTransform();

			if (selectedCount == 1)
			{
				DrawGuiForSingularObject();
			}

			DrawDeleteButton();
		}

		ImGui::End();
	}
}

void PropertiesGuiLayer::DrawGuiForSelectedTransform()
{
	auto& scene = m_MainDataLayer.m_SceneDataLayer.m_SceneCAD;
	auto& selectedGroup = m_MainDataLayer.m_SceneDataLayer.m_SelectedGroup;

	auto initialGroupTransform = selectedGroup.m_GroupTransform;
	auto& groupTransform = selectedGroup.m_GroupTransform;

	std::string labelPrefix = "##scene_object_{}";

	// POSITION LOGIC
	std::string positionLabel = "Position" + labelPrefix;
	ImGui::DragFloat3(positionLabel.c_str(), (float*)&groupTransform.m_Position, 0.1f);

	// ROTATION LOGIC
	std::string rotationLabel = "Rotation" + labelPrefix;
	ImGui::DragFloat3(rotationLabel.c_str(), (float*)&groupTransform.m_Rotation, 0.1f);

	// SCALE LOGIC
	std::string scaleLabel = "Scale" + labelPrefix;
	ImGui::DragFloat3(scaleLabel.c_str(), (float*)&groupTransform.m_Scale, 0.1f);

	if (initialGroupTransform != groupTransform)
	{
		// ----- logic for setting cursor position -----
		auto cursorPosition = groupTransform.m_Position;

		auto lastAction = m_MainDataLayer.m_SceneDataLayer.m_ActionList.Last();
		auto lastActionShared = lastAction.lock();

		if (lastActionShared)
		{
			cursorPosition += lastActionShared->m_Centroid;
		}

		auto& cursor = m_MainDataLayer.m_SceneDataLayer.m_SceneCAD.m_MainCursor;
		cursor->SetPosition(cursorPosition);
		// ---------------------------------------------

		for (auto& id : selectedGroup.m_SelectedObjects)
		{
			auto& object = scene.m_ObjectList[id];
			auto& initialTransform = selectedGroup.m_InitialTransformCopies[id];

			std::unique_ptr<AbstractVisitor> visitor = std::make_unique<ApplyTransformVisitor>(initialTransform, groupTransform, lastActionShared->m_Centroid);
			visitor->Visit(object);
		}
	}

	ImGui::Separator();
}

void PropertiesGuiLayer::DrawGuiForSingularObject()
{
	std::unique_ptr<AbstractVisitor> visitor = std::make_unique<ObjectGuiDrawerVisitor>(m_MainDataLayer.m_SceneDataLayer);
	
	auto& selectedGroup = m_MainDataLayer.m_SceneDataLayer.m_SelectedGroup;
	auto& selectedId = *selectedGroup.m_SelectedObjects.begin();

	auto& object = m_MainDataLayer.m_SceneDataLayer.m_SceneCAD.m_ObjectList[selectedId];

	visitor->Visit(object);
}

void PropertiesGuiLayer::DrawDeleteButton()
{
	auto& selectedGroup = m_MainDataLayer.m_SceneDataLayer.m_SelectedGroup;
	auto groupTransform = selectedGroup.m_GroupTransform;

	auto max = ImGui::GetWindowContentRegionMax();
	auto min = ImGui::GetWindowContentRegionMin();

	auto cursorPos = ImGui::GetCursorPos();
	auto style = ImGui::GetStyle();

	auto buttonHeight = 20 + 2 * style.ItemInnerSpacing.y;
	auto fullHeight = buttonHeight;

	if (max.y - fullHeight > cursorPos.y)
	{
		ImGui::SetCursorPos({ min.x, max.y - fullHeight });
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
