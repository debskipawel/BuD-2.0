#include "ObjectUnselectVisitor.h"

#include <Applications/CAD/Visitors/Transform/ApplyGroupTransformVisitor.h>

ObjectUnselectVisitor::ObjectUnselectVisitor(SceneDataLayerCAD& sceneDataLayer)
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

void ObjectUnselectVisitor::Visit(IntersectionCurve& curve)
{
	UnselectManually(m_Caller);

	curve.m_Color = IntersectionCurve::UNSELECTED_COLOR;

	for (auto& segment : curve.m_InstanceData.m_Segments)
	{
		segment.m_Color = curve.m_Color;
	}
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

void ObjectUnselectVisitor::Visit(BezierPatchC0& patch)
{
	patch.m_Color = BezierPatchC0::UNSELECTED_COLOR;
	patch.m_InstanceData.m_Color = patch.m_Color;

	CommonUnselectPatch(patch);
}

void ObjectUnselectVisitor::Visit(BezierPatchC2& patch)
{
	patch.m_Color = BezierPatchC2::UNSELECTED_COLOR;
	patch.m_InstanceData.m_Color = patch.m_Color;

	CommonUnselectPatch(patch);
}

void ObjectUnselectVisitor::Visit(BezierSurfaceC0& surface)
{
	CommonUnselectSurface(surface);
}

void ObjectUnselectVisitor::Visit(BezierSurfaceC2& surface)
{
	CommonUnselectSurface(surface);
}

void ObjectUnselectVisitor::CommonUnselectCurve(BaseCurve& curve)
{
	UnselectManually(m_Caller);

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

void ObjectUnselectVisitor::CommonUnselectPatch(BaseBezierPatch& patch)
{
	UnselectManually(m_Caller);

	for (auto& controlPoint : patch.m_ControlPoints)
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

void ObjectUnselectVisitor::CommonUnselectSurface(BaseBezierSurface& surface)
{
	UnselectManually(m_Caller);

	for (auto& patch : surface.m_BezierPatches)
	{
		if (!m_SceneDataLayer.m_ManuallySelected.Selected(patch->Id()))
		{
			patch->m_Color = BaseBezierPatch::UNSELECTED_COLOR;
			patch->m_InstanceData.m_Color = BaseBezierPatch::UNSELECTED_COLOR;
		}

		for (auto& controlPoint : patch->m_ControlPoints)
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
