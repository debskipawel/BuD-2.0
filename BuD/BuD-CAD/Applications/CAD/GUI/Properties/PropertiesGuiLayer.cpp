#include "PropertiesGuiLayer.h"

#include <BuD.h>
#include <imgui.h>

#include <Applications/CAD/Visitors/Deletion/ObjectDeletionVisitor.h>
#include <Applications/CAD/Visitors/ObjectGui/ObjectGuiDrawerVisitor.h>

#include <Applications/CAD/Visitors/Transform/AfterUpdateTransformVisitor.h>
#include <Applications/CAD/Visitors/Transform/ApplyGroupTransformVisitor.h>

PropertiesGuiLayer::PropertiesGuiLayer(MainDataLayerCAD& dataLayer)
	: BaseGuiLayerCAD(dataLayer)
{
}

void PropertiesGuiLayer::DrawGui()
{
	if (ImGui::Begin("Properties"))
	{
		auto selectedCount = m_MainDataLayer.m_SceneDataLayer.m_ManuallySelected.Count();

		switch (selectedCount)
		{
			case 0:
			{
				break;
			}
			case 1:
			{
				DrawGuiForSingularObject();
				break;
			}
			default:
			{
				DrawGuiForSelectedTransform();
				break;
			}
		}

		if (selectedCount)
		{
			DrawDeleteButton();
		}

		ImGui::End();
	}
}

bool PropertiesGuiLayer::DrawGuiForTransform(TransformComponent& transform)
{
	auto transformCopy = transform;

	std::string positionLabel = std::format("Position ##transform_position_{}", reinterpret_cast<int>(&transform));
	ImGui::DragFloat3(positionLabel.c_str(), (float*)&transform.m_Position, 0.1f);

	std::string rotationLabel = std::format("Rotation ##transform_rotation_{}", reinterpret_cast<int>(&transform));
	ImGui::DragFloat3(rotationLabel.c_str(), (float*)&transform.m_Rotation, 0.1f);

	std::string scaleLabel = std::format("Scale ##transform_scale_{}", reinterpret_cast<int>(&transform));
	ImGui::DragFloat3(scaleLabel.c_str(), (float*)&transform.m_Scale, 0.1f);

	return transform != transformCopy;
}

void PropertiesGuiLayer::DrawGuiForSelectedTransform()
{
	auto& selectedForTransform = m_MainDataLayer.m_SceneDataLayer.m_SelectedForTransform;
	auto& groupTransform = selectedForTransform.GroupTransform();

	if (DrawGuiForTransform(groupTransform))
	{
		auto centroid = selectedForTransform.Centroid();

		auto& cursor = m_MainDataLayer.m_SceneDataLayer.m_SceneCAD.m_CentroidCursor;
		cursor->SetPosition(centroid);

		auto onTransformVisitor = std::make_unique<ApplyGroupTransformVisitor>(groupTransform, centroid - groupTransform.m_Position);
		std::unique_ptr<AbstractVisitor> afterTransformVisitor = std::make_unique<AfterUpdateTransformVisitor>();

		selectedForTransform.ForEachSelected(
			[&selectedForTransform, &onTransformVisitor](std::shared_ptr<SceneObjectCAD> object)
			{
				auto initialTransform = selectedForTransform.InitialTransform(object->Id());

				onTransformVisitor->SetInitialTransform(initialTransform);
				onTransformVisitor->Visit(object);
			});

		selectedForTransform.ForEachSelected(
			[&afterTransformVisitor](std::shared_ptr<SceneObjectCAD> object)
			{
				afterTransformVisitor->Visit(object);
			}
		);
	}

	if (groupTransform.m_Scale.x == 0.0f || groupTransform.m_Scale.y == 0.0f || groupTransform.m_Scale.z == 0.0f)
	{
		ImGui::TextColored({ 1.0f, 0.0f, 0.0f, 1.0f }, "WARNING: ");
		ImGui::SameLine();
		ImGui::TextWrapped("Zero scale. Selecting/unselecting now may lead to unrecoverable detail loss.");
	}

	ImGui::Separator();
}

void PropertiesGuiLayer::DrawGuiForSingularObject()
{
	auto object = m_MainDataLayer.m_SceneDataLayer.m_ManuallySelected.First();

	std::unique_ptr<AbstractVisitor> visitor = std::make_unique<ObjectGuiDrawerVisitor>(m_MainDataLayer.m_SceneDataLayer);

	visitor->Visit(object);
}

void PropertiesGuiLayer::DrawDeleteButton()
{
	auto max = ImGui::GetWindowContentRegionMax();
	auto min = ImGui::GetWindowContentRegionMin();

	auto cursorPos = ImGui::GetCursorPos();
	auto style = ImGui::GetStyle();

	auto buttonHeight = 20;
	auto buttonHeightWithSpacing = buttonHeight + style.ItemInnerSpacing.y;
	auto fullHeight = buttonHeightWithSpacing;

	auto& manuallySelected = m_MainDataLayer.m_SceneDataLayer.m_ManuallySelected;

	auto addBezierC0 = manuallySelected.Count() > 1 && manuallySelected.ValidatedForControlPoints();
	auto addYukselC2 = manuallySelected.Count() > 1 && manuallySelected.ValidatedForControlPoints();
	auto addBezierC2 = manuallySelected.Count() >= 4 && manuallySelected.ValidatedForControlPoints();

	fullHeight += ((int)addBezierC0 + (int)addYukselC2 + (int)addBezierC2) * buttonHeightWithSpacing;

	if (max.y - fullHeight > cursorPos.y)
	{
		ImGui::SetCursorPos({ min.x, max.y - fullHeight });
	}

	ImGui::Separator();

	if (addBezierC0 && ImGui::Button("Add Bezier C0 curve", ImVec2(max.x - min.x, buttonHeight)))
	{
		std::vector<std::weak_ptr<Point>> controlPoints;

		manuallySelected.ForEachSelected(
			[&controlPoints](std::shared_ptr<SceneObjectCAD> object)
			{
				auto point = std::dynamic_pointer_cast<Point>(object);

				if (point)
				{
					controlPoints.push_back(point);
				}
			});

		m_MainDataLayer.m_SceneDataLayer.m_SceneCAD.CreateBezierCurveC0(controlPoints);
	}

	if (addBezierC2 && ImGui::Button("Add Bezier C2 curve", ImVec2(max.x - min.x, buttonHeight)))
	{
		std::vector<std::weak_ptr<Point>> controlPoints;

		manuallySelected.ForEachSelected(
			[&controlPoints](std::shared_ptr<SceneObjectCAD> object)
			{
				auto point = std::dynamic_pointer_cast<Point>(object);

				if (point)
				{
					controlPoints.push_back(point);
				}
			});

		m_MainDataLayer.m_SceneDataLayer.m_SceneCAD.CreateBezierCurveC2(controlPoints);
	}

	if (addYukselC2 && ImGui::Button("Add interpolating C2 curve", ImVec2(max.x - min.x, buttonHeight)))
	{
		std::vector<std::weak_ptr<Point>> controlPoints;

		manuallySelected.ForEachSelected(
			[&controlPoints](std::shared_ptr<SceneObjectCAD> object)
			{
				auto point = std::dynamic_pointer_cast<Point>(object);

				if (point)
				{
					controlPoints.push_back(point);
				}
			});

		m_MainDataLayer.m_SceneDataLayer.m_SceneCAD.CreateYukselInterpolatingCurveC2(controlPoints);
	}

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

			while (m_MainDataLayer.m_SceneDataLayer.m_ManuallySelected.Count())
			{
				auto object = m_MainDataLayer.m_SceneDataLayer.m_ManuallySelected.First();

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
