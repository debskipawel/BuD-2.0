#include "ObjectSelectVisitor.h"

#include <Visitors/Transform/ApplyGroupTransformVisitor.h>

ObjectSelectVisitor::ObjectSelectVisitor(SceneDataLayer& sceneDataLayer)
	: BaseObjectSelectionVisitor(sceneDataLayer)
{
}

void ObjectSelectVisitor::Visit(Torus& torus)
{
	SelectManually(m_Caller);
	SelectForTransform(m_Caller);

	torus.m_InstanceData.m_Color = Torus::SELECTED_COLOR;
}

void ObjectSelectVisitor::Visit(Point& point)
{
	SelectManually(m_Caller);
	SelectForTransform(m_Caller);

	point.m_InstanceData.m_Color = Point::SELECTED_COLOR;
}

void ObjectSelectVisitor::Visit(BezierCurveC0& curve)
{
	curve.m_Color = BezierCurveC0::SELECTED_COLOR;

	for (auto& segment : curve.m_InstanceData.m_Segments)
	{
		segment.m_Color = BezierCurveC0::SELECTED_COLOR;
	}

	CommonSelectCurve(curve);
}

void ObjectSelectVisitor::Visit(BezierCurveC2& curve)
{
	curve.m_Color = BezierCurveC2::SELECTED_COLOR;

	for (auto& segment : curve.m_InstanceData.m_Segments)
	{
		segment.m_Color = BezierCurveC2::SELECTED_COLOR;
	}

	CommonSelectCurve(curve);
}

void ObjectSelectVisitor::Visit(YukselInterpolatingCurveC2& curve)
{
	curve.m_Color = YukselInterpolatingCurveC2::SELECTED_COLOR;

	for (auto& segment : curve.m_InstanceData.m_Segments)
	{
		segment.m_Color = YukselInterpolatingCurveC2::SELECTED_COLOR;
	}

	CommonSelectCurve(curve);
}

void ObjectSelectVisitor::CommonSelectCurve(BaseCurve& curve)
{
	SelectManually(m_Caller);

	for (auto& controlPoint : curve.m_ControlPoints)
	{
		auto controlPointShared = controlPoint.lock();

		if (!controlPointShared)
		{
			continue;
		}

		if (!m_SceneDataLayer.m_ManuallySelected.Selected(controlPointShared->Id()))
		{
			SelectForTransform(controlPoint);
		}
	}
}

void ObjectSelectVisitor::SelectManually(std::weak_ptr<SceneObjectCAD> object)
{
	auto& selectGroup = m_SceneDataLayer.m_ManuallySelected;
	auto objectShared = object.lock();

	if (!objectShared || selectGroup.Selected(objectShared->Id()))
	{
		return;
	}

	selectGroup.Select(object);
}

void ObjectSelectVisitor::SelectForTransform(std::weak_ptr<SceneObjectCAD> object)
{
	auto& selectGroup = m_SceneDataLayer.m_SelectedForTransform;
	auto objectShared = object.lock();

	if (!objectShared || selectGroup.Selected(objectShared->Id()))
	{
		return;
	}

	selectGroup.Select(object);

	SetCursorToCentroid();
}
