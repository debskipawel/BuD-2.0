#include "ObjectDeletionVisitor.h"

#include <Objects/CAD/PointBased/PointBasedObjectCAD.h>
#include <Visitors/Selection/ObjectUnselectVisitor.h>

ObjectDeletionVisitor::ObjectDeletionVisitor(SceneDataLayer& dataLayer)
	: m_SceneDataLayer(dataLayer)
{
}

void ObjectDeletionVisitor::Visit(Torus& torus)
{
	std::unique_ptr<AbstractVisitor> unselectVisitor = std::make_unique<ObjectUnselectVisitor>(m_SceneDataLayer);
	unselectVisitor->Visit(m_Caller);

	m_SceneDataLayer.m_SceneCAD.DeleteObject(torus);
}

void ObjectDeletionVisitor::Visit(Point& point)
{
	for (auto& object : point.m_PointBasedObjects)
	{
		auto objectShared = object.lock();

		if (!objectShared)
		{
			continue;
		}

		objectShared->OnPointRemove(point.Id());
	}

	std::unique_ptr<AbstractVisitor> unselectVisitor = std::make_unique<ObjectUnselectVisitor>(m_SceneDataLayer);
	unselectVisitor->Visit(m_Caller);

	m_SceneDataLayer.m_SceneCAD.DeleteObject(point);
}
