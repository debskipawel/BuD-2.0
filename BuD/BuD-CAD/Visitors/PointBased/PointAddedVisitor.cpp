#include "PointAddedVisitor.h"

PointAddedVisitor::PointAddedVisitor(MainDataLayer& dataLayer, std::weak_ptr<Point> pointAdded)
	: m_MainDataLayer(dataLayer)
{
	m_PointAdded = pointAdded.lock();
}

void PointAddedVisitor::Visit(std::weak_ptr<SceneObjectCAD> object)
{
	if (!m_PointAdded)
	{
		return;
	}

	AbstractVisitor::Visit(object);
}

void PointAddedVisitor::Visit(BezierCurveC0& curve)
{
	curve.OnPointAdd(m_PointAdded);

	auto pointBasedObject = std::dynamic_pointer_cast<PointBasedObjectCAD>(m_Caller.lock());

	m_PointAdded->m_PointBasedObjects.push_back(pointBasedObject);

	UpdateCentroidCursor();
}

void PointAddedVisitor::Visit(BezierCurveC2& curve)
{
	curve.OnPointAdd(m_PointAdded);

	auto pointBasedObject = std::dynamic_pointer_cast<PointBasedObjectCAD>(m_Caller.lock());

	m_PointAdded->m_PointBasedObjects.push_back(pointBasedObject);

	UpdateCentroidCursor();
}

void PointAddedVisitor::Visit(YukselInterpolatingCurveC2& curve)
{
	curve.OnPointAdd(m_PointAdded);

	auto pointBasedObject = std::dynamic_pointer_cast<PointBasedObjectCAD>(m_Caller.lock());

	m_PointAdded->m_PointBasedObjects.push_back(pointBasedObject);

	UpdateCentroidCursor();
}

void PointAddedVisitor::UpdateCentroidCursor()
{
	auto& selectedTransform = m_MainDataLayer.m_SceneDataLayer.m_SelectedForTransform;
	auto& cursor = m_MainDataLayer.m_SceneDataLayer.m_SceneCAD.m_CentroidCursor;

	selectedTransform.Select(m_PointAdded);

	cursor->SetPosition(selectedTransform.Centroid());
}
