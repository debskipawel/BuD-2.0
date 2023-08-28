#include "ObjectDeletionVisitor.h"

#include <Objects/CAD/PointBased/PointBasedObjectCAD.h>
#include <Visitors/Selection/ObjectUnselectVisitor.h>
#include <Visitors/PointBased/PointRemovedVisitor.h>

ObjectDeletionVisitor::ObjectDeletionVisitor(SceneDataLayer& dataLayer)
	: m_SceneDataLayer(dataLayer)
{
}

void ObjectDeletionVisitor::Visit(Torus& torus)
{
	std::unique_ptr<AbstractVisitor> unselectVisitor = std::make_unique<ObjectUnselectVisitor>(m_SceneDataLayer);
	unselectVisitor->Visit(m_Caller);

	for (auto& [id, intersectionCurve] : torus.m_IntersectionCurves)
	{
		std::weak_ptr<SceneObjectCAD> intersectionCurveShared = intersectionCurve.lock();

		AbstractVisitor::Visit(intersectionCurveShared);
	}

	m_SceneDataLayer.m_SceneCAD.DeleteObject(torus);
}

void ObjectDeletionVisitor::Visit(Point& point)
{
	auto pointShared = std::dynamic_pointer_cast<Point>(m_Caller.lock());
	std::unique_ptr<AbstractVisitor> pointDeletionVisitor = std::make_unique<PointRemovedVisitor>(m_SceneDataLayer, pointShared);

	for (auto& object : point.m_PointBasedObjects)
	{
		auto objectShared = object.lock();

		if (!objectShared)
		{
			continue;
		}

		pointDeletionVisitor->Visit(objectShared);
	}

	std::unique_ptr<AbstractVisitor> unselectVisitor = std::make_unique<ObjectUnselectVisitor>(m_SceneDataLayer);
	unselectVisitor->Visit(m_Caller);

	m_SceneDataLayer.m_SceneCAD.DeleteObject(point);
}

void ObjectDeletionVisitor::Visit(IntersectionCurve& curve)
{
	auto surface = std::dynamic_pointer_cast<ParameterizedObject2D>(curve.m_Surface.lock());

	if (surface)
	{
		surface->RemoveIntersectionCurve(std::dynamic_pointer_cast<IntersectionCurve>(m_Caller.lock()));
	}

	std::unique_ptr<AbstractVisitor> unselectVisitor = std::make_unique<ObjectUnselectVisitor>(m_SceneDataLayer);
	unselectVisitor->Visit(m_Caller);

	m_SceneDataLayer.m_SceneCAD.DeleteObject(curve);
}

void ObjectDeletionVisitor::Visit(BezierCurveC0& curve)
{
	NotifyControlPointsAboutDeletion(curve);

	std::unique_ptr<AbstractVisitor> unselectVisitor = std::make_unique<ObjectUnselectVisitor>(m_SceneDataLayer);
	unselectVisitor->Visit(m_Caller);

	m_SceneDataLayer.m_SceneCAD.DeleteObject(curve);
}

void ObjectDeletionVisitor::Visit(BezierCurveC2& curve)
{
	NotifyControlPointsAboutDeletion(curve);

	std::unique_ptr<AbstractVisitor> unselectVisitor = std::make_unique<ObjectUnselectVisitor>(m_SceneDataLayer);
	unselectVisitor->Visit(m_Caller);

	m_SceneDataLayer.m_SceneCAD.DeleteObject(curve);
}

void ObjectDeletionVisitor::Visit(YukselInterpolatingCurveC2& curve)
{
	NotifyControlPointsAboutDeletion(curve);

	std::unique_ptr<AbstractVisitor> unselectVisitor = std::make_unique<ObjectUnselectVisitor>(m_SceneDataLayer);
	unselectVisitor->Visit(m_Caller);

	m_SceneDataLayer.m_SceneCAD.DeleteObject(curve);
}

void ObjectDeletionVisitor::Visit(BezierPatchC0& patch)
{
	NotifyControlPointsAboutDeletion(patch);

	std::unique_ptr<AbstractVisitor> unselectVisitor = std::make_unique<ObjectUnselectVisitor>(m_SceneDataLayer);
	unselectVisitor->Visit(m_Caller);

	m_SceneDataLayer.m_SceneCAD.DeleteObject(patch);
}

void ObjectDeletionVisitor::Visit(BezierPatchC2& patch)
{
	NotifyControlPointsAboutDeletion(patch);

	std::unique_ptr<AbstractVisitor> unselectVisitor = std::make_unique<ObjectUnselectVisitor>(m_SceneDataLayer);
	unselectVisitor->Visit(m_Caller);

	m_SceneDataLayer.m_SceneCAD.DeleteObject(patch);
}

void ObjectDeletionVisitor::Visit(BezierSurfaceC0& surface)
{
	std::unique_ptr<AbstractVisitor> unselectVisitor = std::make_unique<ObjectUnselectVisitor>(m_SceneDataLayer);
	unselectVisitor->Visit(m_Caller);

	for (auto& [id, intersectionCurve] : surface.m_IntersectionCurves)
	{
		std::weak_ptr<SceneObjectCAD> intersectionCurveShared = intersectionCurve.lock();

		AbstractVisitor::Visit(intersectionCurveShared);
	}

	for (auto& bezierPatch : surface.m_BezierPatches)
	{
		std::weak_ptr<SceneObjectCAD> sceneObject = bezierPatch.lock();
		
		AbstractVisitor::Visit(sceneObject);
	}

	m_SceneDataLayer.m_SceneCAD.DeleteObject(surface);
}

void ObjectDeletionVisitor::Visit(BezierSurfaceC2& surface)
{
	std::unique_ptr<AbstractVisitor> unselectVisitor = std::make_unique<ObjectUnselectVisitor>(m_SceneDataLayer);
	unselectVisitor->Visit(m_Caller);

	for (auto& [id, intersectionCurve] : surface.m_IntersectionCurves)
	{
		std::weak_ptr<SceneObjectCAD> intersectionCurveShared = intersectionCurve.lock();

		AbstractVisitor::Visit(intersectionCurveShared);
	}

	for (auto& bezierPatch : surface.m_BezierPatches)
	{
		std::weak_ptr<SceneObjectCAD> sceneObject = bezierPatch.lock();

		AbstractVisitor::Visit(sceneObject);
	}

	m_SceneDataLayer.m_SceneCAD.DeleteObject(surface);
}

void ObjectDeletionVisitor::NotifyControlPointsAboutDeletion(PointBasedObjectCAD& object)
{
	auto id = object.Id();

	for (auto& controlPoint : object.m_ControlPoints)
	{
		auto controlPointShared = controlPoint.lock();

		if (controlPointShared)
		{
			auto& dependentObjects = controlPointShared->m_PointBasedObjects;

			std::erase_if(dependentObjects, [id](std::weak_ptr<PointBasedObjectCAD> object)
				{
					auto objectShared = object.lock();
					return !objectShared || objectShared->Id() == id;
				}
			);
		}
	}
}
