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

void ObjectSelectVisitor::Visit(BezierPatchC0& patch)
{
	patch.m_Color = BezierPatchC0::SELECTED_COLOR;
	patch.m_InstanceData.m_Color = patch.m_Color;

	CommonSelectPatch(patch);
}

void ObjectSelectVisitor::Visit(BezierPatchC2& patch)
{
	patch.m_Color = BezierPatchC2::SELECTED_COLOR;
	patch.m_InstanceData.m_Color = patch.m_Color;

	CommonSelectPatch(patch);
}

void ObjectSelectVisitor::Visit(BezierSurfaceC0& surface)
{
	CommonSelectSurface(surface);
}

void ObjectSelectVisitor::Visit(BezierSurfaceC2& surface)
{
	CommonSelectSurface(surface);
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

void ObjectSelectVisitor::CommonSelectPatch(BaseBezierPatch& patch)
{
	SelectManually(m_Caller);

	for (auto& controlPoint : patch.m_ControlPoints)
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

void ObjectSelectVisitor::CommonSelectSurface(BaseBezierSurface& surface)
{
	SelectManually(m_Caller);

	for (auto& patch : surface.m_BezierPatches)
	{
		auto patchShared = patch.lock();

		if (!patchShared)
		{
			continue;
		}

		patchShared->m_Color = BaseBezierPatch::SELECTED_COLOR;
		patchShared->m_InstanceData.m_Color = BaseBezierPatch::SELECTED_COLOR;
	}

	for (auto& controlPoint : surface.m_ControlPoints)
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
