#include "ObjectGuiDrawerVisitor.h"

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

#include <Objects/CAD/PointBased/PointBasedObjectCAD.h>

#include <Visitors/Transform/ApplyGroupTransformVisitor.h>
#include <Visitors/Transform/UpdateTransformVisitor.h>

ObjectGuiDrawerVisitor::ObjectGuiDrawerVisitor(SceneDataLayer& dataLayer)
	: m_SceneDataLayer(dataLayer)
{
}

void ObjectGuiDrawerVisitor::Visit(Torus& torus)
{
	DrawGuiForTag(torus);

	ImGui::Separator();

	if (DrawGuiForTransform(torus.m_Transform))
	{
		std::unique_ptr<AbstractVisitor> visitor = std::make_unique<UpdateTransformVisitor>();

		visitor->Visit(m_Caller);
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
}

void ObjectGuiDrawerVisitor::Visit(Point& point)
{
	DrawGuiForTag(point);

	ImGui::Separator();

	auto& transform = point.m_Transform;
	auto position = transform.m_Position;

	ImGui::DragFloat3("Position", (float*)&transform.m_Position, 0.1f);

	if (position != transform.m_Position)
	{
		std::unique_ptr<AbstractVisitor> visitor = std::make_unique<UpdateTransformVisitor>();

		visitor->Visit(m_Caller);
	}
}

void ObjectGuiDrawerVisitor::Visit(BezierCurveC0& curve)
{
	DrawGuiForSelectedTransform();
	
	ImGui::Separator();

	DrawGuiForTag(curve);

	ImGui::Separator();

	auto borderOn = curve.RenderBorder();
	ImGui::Checkbox("Toggle border", &borderOn);

	curve.RenderBorder(borderOn);
}

void ObjectGuiDrawerVisitor::DrawGuiForSelectedTransform()
{
	auto& selectedForTransform = m_SceneDataLayer.m_SelectedForTransform;
	auto& groupTransform = selectedForTransform.GroupTransform();

	if (DrawGuiForTransform(groupTransform))
	{
		auto centroid = selectedForTransform.Centroid();

		auto& cursor = m_SceneDataLayer.m_SceneCAD.m_MainCursor;
		cursor->SetPosition(centroid);

		selectedForTransform.ForEachSelected(
			[centroid, &groupTransform, &selectedForTransform](std::shared_ptr<SceneObjectCAD> object)
			{
				auto initialTransform = selectedForTransform.InitialTransform(object->Id());

				std::unique_ptr<AbstractVisitor> visitor = std::make_unique<ApplyGroupTransformVisitor>(initialTransform, groupTransform, centroid - groupTransform.m_Position);
				visitor->Visit(object);
			});
	}
}

bool ObjectGuiDrawerVisitor::DrawGuiForTag(SceneObjectCAD& object)
{
	auto& tag = object.m_Tag;
	return ImGui::InputText("Tag###tag", &tag);
}

bool ObjectGuiDrawerVisitor::DrawGuiForTransform(TransformComponent& transform)
{
	bool changeFlag = false;

	std::string labelPrefix = "##scene_object";
	
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
