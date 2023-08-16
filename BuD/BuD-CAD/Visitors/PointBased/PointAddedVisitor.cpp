#include "PointAddedVisitor.h"

PointAddedVisitor::PointAddedVisitor(MainDataLayer& dataLayer, std::weak_ptr<Point> pointAdded)
	: m_MainDataLayer(dataLayer)
{
	m_PointAdded = pointAdded.lock();
}

void PointAddedVisitor::Visit(BezierCurveC0& curve)
{
	if (!m_PointAdded)
	{
		return;
	}

	curve.OnPointAdd(m_PointAdded);

	auto pointBasedObject = std::dynamic_pointer_cast<PointBasedObjectCAD>(m_Caller.lock());

	m_PointAdded->m_PointBasedObjects.push_back(pointBasedObject);

	auto& selectedTransform = m_MainDataLayer.m_SceneDataLayer.m_SelectedForTransform;
	auto& cursor = m_MainDataLayer.m_SceneDataLayer.m_SceneCAD.m_CentroidCursor;

	selectedTransform.Select(m_PointAdded);
	
	cursor->SetPosition(selectedTransform.Centroid());
}

void PointAddedVisitor::Visit(BezierCurveC2& curve)
{
	if (!m_PointAdded)
	{
		return;
	}

	curve.OnPointAdd(m_PointAdded);

	auto pointBasedObject = std::dynamic_pointer_cast<PointBasedObjectCAD>(m_Caller.lock());

	m_PointAdded->m_PointBasedObjects.push_back(pointBasedObject);

	auto& selectedTransform = m_MainDataLayer.m_SceneDataLayer.m_SelectedForTransform;
	auto& cursor = m_MainDataLayer.m_SceneDataLayer.m_SceneCAD.m_CentroidCursor;

	selectedTransform.Select(m_PointAdded);

	cursor->SetPosition(selectedTransform.Centroid());
}
