#include "ObjectGuiDrawerVisitor.h"

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

ObjectGuiDrawerVisitor::ObjectGuiDrawerVisitor(PropertiesViewModel& viewModel)
	: m_ViewModel(viewModel)
{
}

void ObjectGuiDrawerVisitor::Visit(Torus& torus)
{
	DrawGuiForTag(torus);

	ImGui::Separator();

	if (DrawGuiForTransform(torus))
	{
		const auto& transform = torus.m_Transform;

		auto rotation = transform.m_Rotation;
		rotation.x = DirectX::XMConvertToRadians(rotation.x);
		rotation.y = DirectX::XMConvertToRadians(rotation.y);
		rotation.z = DirectX::XMConvertToRadians(rotation.z);

		auto model = dxm::Matrix::CreateScale(transform.m_Scale) * dxm::Matrix::CreateFromYawPitchRoll(rotation) * dxm::Matrix::CreateTranslation(transform.m_Position);

		torus.m_InstanceData.m_ModelMatrix = model;
	}

	ImGui::Separator();

	auto& instanceData = torus.m_InstanceData;

	ImGui::DragInt2("Segments ###torus_segments", (int*)&instanceData.m_SegmentsU, 1.0f, 3);
	instanceData.m_SegmentsU = max(instanceData.m_SegmentsU, 3);
	instanceData.m_SegmentsV = max(instanceData.m_SegmentsV, 3);

	ImGui::DragFloat("Outer radius ###torus_outer", &instanceData.m_OuterRadius, 0.1f, instanceData.m_InnerRadius);
	ImGui::DragFloat("Inner radius ###torus_inner", &instanceData.m_InnerRadius, 0.1f, 0.0f, instanceData.m_OuterRadius);

	instanceData.m_OuterRadius = max(instanceData.m_OuterRadius, instanceData.m_InnerRadius);
	instanceData.m_InnerRadius = min(instanceData.m_OuterRadius, instanceData.m_InnerRadius);

	ImGui::Separator();

	DrawDeleteButton(torus);
}

void ObjectGuiDrawerVisitor::Visit(Cube& cube)
{
}

bool ObjectGuiDrawerVisitor::DrawGuiForTag(SceneObjectCAD& object)
{
	auto& tag = object.m_Tag;
	return ImGui::InputText("Tag###tag", &tag);
}

bool ObjectGuiDrawerVisitor::DrawGuiForTransform(SceneObjectCAD& object)
{
	bool changeFlag = false;

	auto& transform = object.m_Transform;

	auto labelPrefix = std::format("##scene_object_{}", object.Id());
	
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

void ObjectGuiDrawerVisitor::DrawDeleteButton(SceneObjectCAD& object)
{
	auto max = ImGui::GetWindowContentRegionMax();
	auto min = ImGui::GetWindowContentRegionMin();

	auto cursorPos = ImGui::GetCursorPos();

	if (max.y - 20 > cursorPos.y)
	{
		ImGui::SetCursorPos({ min.x, max.y - 20 });
	}

	if (ImGui::Button("Delete", ImVec2(max.x - min.x, 20)))
	{
		auto& scene = m_ViewModel.m_SceneCAD;
		scene.DeleteObject(object);
	}
}
