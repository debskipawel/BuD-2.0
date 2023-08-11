#include "PropertiesGuiLayer.h"

#include <BuD.h>
#include <imgui.h>

#include "PerformanceGuiLayer.h"
#include "ObjectGuiDrawerVisitor.h"

PropertiesGuiLayer::PropertiesGuiLayer(PropertiesViewModel& properties, AppStateViewModel& appState)
	: m_Properties(properties), m_AppState(appState)
{
	m_PerformanceGuiLayer = std::make_unique<PerformanceGuiLayer>();
}

void PropertiesGuiLayer::DrawGui()
{
	if (ImGui::Begin("Properties"))
	{
		auto& composite = m_Properties.m_SceneCAD.m_SelectedGroup;

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

		m_PerformanceGuiLayer->DrawGui();

		ImGui::End();
	}
}

void PropertiesGuiLayer::DrawGuiForSingularObject()
{
	std::unique_ptr<AbstractVisitor> visitor = std::make_unique<ObjectGuiDrawerVisitor>(m_Properties);
	auto& composite = m_Properties.m_SceneCAD.m_SelectedGroup;

	auto& [key, object] = *composite.m_Objects.begin();

	auto objectShared = object.lock();

	visitor->Visit(*objectShared);
}

void PropertiesGuiLayer::DrawGuiForComposite()
{
	auto& composite = m_Properties.m_SceneCAD.m_SelectedGroup;

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
		m_AppState.Freeze();
		ImGui::OpenPopup("Delete selected ###delete_selected_popup");
	}

	if (ImGui::BeginPopupModal("Delete all selected? ###delete_selected_popup", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize))
	{
		auto max = ImGui::GetWindowContentRegionMax();
		auto min = ImGui::GetWindowContentRegionMin();

		if (ImGui::Button("Yes###delete_yes_button", { 150, 0 }))
		{
			auto& scene = m_Properties.m_SceneCAD;
			scene.DeleteSelected();
			ImGui::CloseCurrentPopup();
			m_AppState.Unfreeze();
		}

		if (ImGui::Button("No###delete_no_button", { 150, 0 }))
		{
			ImGui::CloseCurrentPopup();
			m_AppState.Unfreeze();
		}

		ImGui::EndPopup();
	}
}

bool PropertiesGuiLayer::DrawGuiForTransform(TransformComponent& transform)
{
	bool changeFlag = false;

	std::string labelPrefix = "##transform";

	// POSITION LOGIC
	std::string positionLabel = "Position" + labelPrefix;
	auto positionCopy = transform.m_Position;

	ImGui::DragFloat3(positionLabel.c_str(), (float*)&transform.m_Position, 0.1f);

	changeFlag = changeFlag || (transform.m_Position != positionCopy);

	// ROTATION LOGIC
	std::string rotationLabel = "Rotation" + labelPrefix;

	auto rotationCopy = transform.m_Rotation;

	ImGui::DragFloat3(rotationLabel.c_str(), (float*)&transform.m_Rotation);

	changeFlag = changeFlag || (transform.m_Rotation != rotationCopy);

	// SCALE LOGIC
	std::string scaleLabel = "Scale" + labelPrefix;
	auto scaleCopy = transform.m_Scale;

	ImGui::DragFloat3(scaleLabel.c_str(), (float*)&transform.m_Scale, 0.1f);

	changeFlag = changeFlag || (transform.m_Scale != scaleCopy);

	return changeFlag;
}
