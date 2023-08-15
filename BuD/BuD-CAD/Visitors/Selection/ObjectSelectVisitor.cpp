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

	SetCursorToCentroid();

	torus.m_InstanceData.m_Color = Torus::SELECTED_COLOR;
}

void ObjectSelectVisitor::Visit(Point& point)
{
	SelectManually(m_Caller);
	SelectForTransform(m_Caller);

	SetCursorToCentroid();

	point.m_InstanceData.m_Color = Point::SELECTED_COLOR;
}

void ObjectSelectVisitor::Visit(BezierCurveC0& curve)
{
	SelectManually(m_Caller);

	curve.m_Color = BezierCurveC0::SELECTED_COLOR;

	for (auto& segment : curve.m_InstanceData.m_Segments)
	{
		segment.m_Color = BezierCurveC0::SELECTED_COLOR;
	}

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

	if (curve.m_ControlPoints.size())
	{
		SetCursorToCentroid();
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
}
