#include "ObjectUnselectVisitor.h"

#include <Visitors/Transform/ApplyGroupTransformVisitor.h>

ObjectUnselectVisitor::ObjectUnselectVisitor(SceneDataLayer& sceneDataLayer)
	: BaseObjectSelectionVisitor(sceneDataLayer)
{
}

void ObjectUnselectVisitor::Visit(Torus& torus)
{
	UnselectForTransform(m_Caller);
	UnselectManually(m_Caller);

	torus.m_InstanceData.m_Color = Torus::UNSELECTED_COLOR;
}

void ObjectUnselectVisitor::Visit(Point& point)
{
	UnselectForTransform(m_Caller);
	UnselectManually(m_Caller);

	point.m_InstanceData.m_Color = Point::UNSELECTED_COLOR;
}

void ObjectUnselectVisitor::Visit(BezierCurveC0& curve)
{
	curve.m_Color = BezierCurveC0::UNSELECTED_COLOR;

	for (auto& segment : curve.m_InstanceData.m_Segments)
	{
		segment.m_Color = BezierCurveC0::UNSELECTED_COLOR;
	}

	CommonUnselectCurve(curve);
}

void ObjectUnselectVisitor::Visit(BezierCurveC2& curve)
{
	curve.m_Color = BezierCurveC2::UNSELECTED_COLOR;

	for (auto& segment : curve.m_InstanceData.m_Segments)
	{
		segment.m_Color = BezierCurveC2::UNSELECTED_COLOR;
	}

	CommonUnselectCurve(curve);
}

void ObjectUnselectVisitor::Visit(YukselInterpolatingCurveC2& curve)
{
	curve.m_Color = YukselInterpolatingCurveC2::UNSELECTED_COLOR;

	for (auto& segment : curve.m_InstanceData.m_Segments)
	{
		segment.m_Color = YukselInterpolatingCurveC2::UNSELECTED_COLOR;
	}

	CommonUnselectCurve(curve);
}

void ObjectUnselectVisitor::CommonUnselectCurve(BaseCurve& curve)
{
	UnselectManually(m_Caller);

	curve.RenderControlPointBorder(false);

	for (auto& controlPoint : curve.m_ControlPoints)
	{
		auto controlPointShared = controlPoint.lock();

		if (!controlPointShared)
		{
			continue;
		}

		if (!m_SceneDataLayer.m_ManuallySelected.Selected(controlPointShared->Id()))
		{
			UnselectForTransform(controlPoint);
		}
	}
}

void ObjectUnselectVisitor::UnselectManually(std::weak_ptr<SceneObjectCAD> object)
{
	auto& selectGroup = m_SceneDataLayer.m_ManuallySelected;
	auto objectShared = object.lock();

	if (!objectShared || !selectGroup.Selected(objectShared->Id()))
	{
		return;
	}

	selectGroup.Unselect(objectShared->Id());
}

void ObjectUnselectVisitor::UnselectForTransform(std::weak_ptr<SceneObjectCAD> object)
{
	auto& selectGroup = m_SceneDataLayer.m_SelectedForTransform;
	auto objectShared = object.lock();

	if (!objectShared || !selectGroup.Selected(objectShared->Id()))
	{
		return;
	}

	selectGroup.Unselect(objectShared->Id());

	SetCursorToCentroid();
}
