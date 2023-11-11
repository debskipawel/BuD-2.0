#include "AfterUpdateTransformVisitor.h"

void AfterUpdateTransformVisitor::Visit(Torus& torus)
{
	for (auto& [id, intersectionCurve] : torus.m_IntersectionCurves)
	{
		auto curveShared = intersectionCurve.lock();

		if (!curveShared || m_ModifiedObjects.contains(curveShared->Id()))
		{
			continue;
		}

		m_ModifiedObjects.insert(curveShared->Id());

		curveShared->UpdateInstanceData();
	}
}

void AfterUpdateTransformVisitor::Visit(Point& point)
{
	for (auto& dependentObject : point.m_PointBasedObjects)
	{
		auto dependentObjectShared = dependentObject.lock();

		if (!dependentObjectShared || m_ModifiedObjects.contains(dependentObjectShared->Id()))
		{
			continue;
		}

		m_ModifiedObjects.insert(dependentObjectShared->Id());

		dependentObjectShared->OnPointModify();
	}
}
