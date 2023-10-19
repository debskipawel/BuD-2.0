#include "ObjectGuiDrawerVisitor.h"

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

#include <Applications/CAD/Objects/CAD/PointBased/PointBasedObjectCAD.h>

#include <Applications/CAD/Visitors/Intersection/CalculatorPointOnSurface.h>

#include <Applications/CAD/Visitors/Transform/ApplyGroupTransformVisitor.h>
#include <Applications/CAD/Visitors/Transform/AfterUpdateTransformVisitor.h>
#include <Applications/CAD/Visitors/Transform/UpdateTransformVisitor.h>

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
		std::unique_ptr<AbstractVisitor> onTransformVisitor = std::make_unique<UpdateTransformVisitor>();
		std::unique_ptr<AbstractVisitor> afterTransformVisitor = std::make_unique<AfterUpdateTransformVisitor>();
		
		onTransformVisitor->Visit(m_Caller);
		afterTransformVisitor->Visit(m_Caller);

		auto centroid = m_SceneDataLayer.m_SelectedForTransform.Centroid();

		auto& cursor = m_SceneDataLayer.m_SceneCAD.m_CentroidCursor;
		cursor->SetPosition(centroid);
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

	DrawGuiForParameterSpace(torus);
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
		std::unique_ptr<AbstractVisitor> onTransformVisitor = std::make_unique<UpdateTransformVisitor>();
		std::unique_ptr<AbstractVisitor> afterTransformVisitor = std::make_unique<AfterUpdateTransformVisitor>();
		
		onTransformVisitor->Visit(m_Caller);
		afterTransformVisitor->Visit(m_Caller);

		auto centroid = m_SceneDataLayer.m_SelectedForTransform.Centroid();

		auto& cursor = m_SceneDataLayer.m_SceneCAD.m_CentroidCursor;
		cursor->SetPosition(centroid);
	}
}

void ObjectGuiDrawerVisitor::Visit(IntersectionCurve& curve)
{
	DrawGuiForTag(curve);

	ImGui::Separator();

	auto width = ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x;

	if (ImGui::Button("Make interpolation curve", { width, 0 }))
	{
		const auto& intersectionPoints = curve.IntersectionPoints();

		std::vector<std::weak_ptr<Point>> controlPoints = {};

		std::unique_ptr<CalculatorParameterized> pointCalculator = std::make_unique<CalculatorPointOnSurface>();

		for (int i = 0; i < intersectionPoints.size() - 1; i++)
		{
			pointCalculator->SetParameter(intersectionPoints[i].m_Parameter);
			pointCalculator->Visit(curve.m_Surface);

			auto point = m_SceneDataLayer.m_SceneCAD.CreatePoint(pointCalculator->Result());
			controlPoints.push_back(std::dynamic_pointer_cast<Point>(point.lock()));
		}

		if (intersectionPoints.front().m_Parameter == intersectionPoints.back().m_Parameter)
		{
			controlPoints.push_back(controlPoints.front());
		}
		else
		{
			auto parameter = intersectionPoints.back().m_Parameter;

			pointCalculator->SetParameter(parameter);
			pointCalculator->Visit(curve.m_Surface);

			auto point = m_SceneDataLayer.m_SceneCAD.CreatePoint(pointCalculator->Result());
			controlPoints.push_back(std::dynamic_pointer_cast<Point>(point.lock()));
		}

		m_SceneDataLayer.m_SceneCAD.CreateYukselInterpolatingCurveC2(controlPoints);
	}
}

void ObjectGuiDrawerVisitor::Visit(BezierCurveC0& curve)
{
	DrawGuiForTag(curve);

	ImGui::Separator();

	DrawGuiForSelectedTransform();
	
	ImGui::Separator();

	auto borderOn = curve.ShouldDisplayPolygon();
	
	if (ImGui::Checkbox("Toggle polygon ###curve_polygon", &borderOn))
	{
		curve.TogglePolygon(borderOn);
	}
}

void ObjectGuiDrawerVisitor::Visit(BezierCurveC2& curve)
{
	DrawGuiForTag(curve);

	ImGui::Separator();

	DrawGuiForSelectedTransform();

	ImGui::Separator();

	auto borderOn = curve.ShouldDisplayPolygon();
	
	if (ImGui::Checkbox("Toggle polygon ###curve_polygon", &borderOn))
	{
		curve.TogglePolygon(borderOn);
	}
}

void ObjectGuiDrawerVisitor::Visit(YukselInterpolatingCurveC2& curve)
{
	DrawGuiForTag(curve);

	ImGui::Separator();

	DrawGuiForSelectedTransform();
}

void ObjectGuiDrawerVisitor::Visit(BezierPatchC0& patch)
{
	DrawGuiForTag(patch);

	ImGui::Separator();

	DrawGuiForSelectedTransform();

	ImGui::Separator();

	auto polygonOn = patch.ShouldDisplayPolygon();

	if (ImGui::Checkbox("Toggle Polygon ###patch_polygon", &polygonOn))
	{
		patch.TogglePolygon(polygonOn);
	}
}

void ObjectGuiDrawerVisitor::Visit(BezierPatchC2& patch)
{
	DrawGuiForTag(patch);

	ImGui::Separator();

	DrawGuiForSelectedTransform();

	ImGui::Separator();

	auto polygonOn = patch.ShouldDisplayPolygon();

	if (ImGui::Checkbox("Toggle Polygon ###patch_polygon", &polygonOn))
	{
		patch.TogglePolygon(polygonOn);
	}
}

void ObjectGuiDrawerVisitor::Visit(BezierSurfaceC0& surface)
{
	DrawGuiForTag(surface);
	
	ImGui::Separator();

	DrawGuiForSelectedTransform();

	ImGui::Separator();

	auto polygonOn = surface.ShouldDisplayPolygon();

	if (ImGui::Checkbox("Toggle Bezier polygon ###surface_polygon", &polygonOn))
	{
		surface.TogglePolygon(polygonOn);
	}

	ImGui::Separator();

	DrawGuiForParameterSpace(surface);
}

void ObjectGuiDrawerVisitor::Visit(BezierSurfaceC2& surface)
{
	DrawGuiForTag(surface);

	ImGui::Separator();

	DrawGuiForSelectedTransform();

	ImGui::Separator();

	auto polygonOn = surface.ShouldDisplayPolygon();

	if (ImGui::Checkbox("Toggle De Boor polygon ###surface_polygon", &polygonOn))
	{
		surface.TogglePolygon(polygonOn);
	}

	ImGui::Separator();

	DrawGuiForParameterSpace(surface);
}

void ObjectGuiDrawerVisitor::DrawGuiForSelectedTransform()
{
	auto& selectedForTransform = m_SceneDataLayer.m_SelectedForTransform;
	auto& groupTransform = selectedForTransform.GroupTransform();

	if (DrawGuiForTransform(groupTransform))
	{
		auto centroid = selectedForTransform.Centroid();

		auto& cursor = m_SceneDataLayer.m_SceneCAD.m_CentroidCursor;
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
}

void ObjectGuiDrawerVisitor::DrawGuiForParameterSpace(ParameterizedObject2D& parameterized)
{
	constexpr dxm::Vector4 WHITE_COLOR = { 1.0f, 1.0f, 1.0f, 1.0f };
	constexpr dxm::Vector4 BLACK_COLOR = { 0.0f, 0.0f, 0.0f, 1.0f };

	static const std::map<dxm::Vector4, dxm::Vector4> colorMap =
	{
		{ WHITE_COLOR, BLACK_COLOR },
		{ BLACK_COLOR, WHITE_COLOR },
	};

	if (parameterized.m_ParameterSpace.has_value() && ImGui::CollapsingHeader("Parameter space ###parameter_space"))
	{
		ImGui::TextWrapped("Click on any part of the parameter space to trim.");

		auto cursorPreImage = ImGui::GetCursorScreenPos();
		
		auto width = ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x;
		auto imageSize = ImVec2{ width, width };

		ImGui::Image(parameterized.m_ParameterSpace->SRV(), imageSize);

		if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && ImGui::IsItemHovered())
		{
			auto mousePos = ImGui::GetMousePos();

			auto x = mousePos.x - cursorPreImage.x;
			auto y = mousePos.y - cursorPreImage.y;

			auto screenSpaceX = std::clamp(static_cast<float>(x) / imageSize.x, 0.0f, 1.0f);
			auto screenSpaceY = std::clamp(static_cast<float>(y) / imageSize.y, 0.0f, 1.0f);

			parameterized.m_ParameterSpace->BeginEdit();

			auto sampledColor = parameterized.m_ParameterSpace->Sample(screenSpaceX, screenSpaceY);
			
			if (colorMap.contains(sampledColor))
			{
				const auto& fillColor = colorMap.at(sampledColor);

				parameterized.m_ParameterSpace->FloodFill(screenSpaceX, screenSpaceY, fillColor);
			}

			parameterized.m_ParameterSpace->EndEdit();
		}
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
