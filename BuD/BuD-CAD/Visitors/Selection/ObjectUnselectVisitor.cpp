#include "ObjectUnselectVisitor.h"

#include <Visitors/Transform/ApplyTransformVisitor.h>

ObjectUnselectVisitor::ObjectUnselectVisitor(SceneDataLayer& sceneDataLayer)
	: BaseObjectSelectionVisitor(sceneDataLayer)
{
}

void ObjectUnselectVisitor::Visit(Torus& torus)
{
	UniversalUnselect(torus);

	torus.m_InstanceData.m_Color = Torus::UNSELECTED_COLOR;
}

void ObjectUnselectVisitor::Visit(Point& point)
{
	UniversalUnselect(point);

	point.m_InstanceData.m_Color = Torus::UNSELECTED_COLOR;
}

void ObjectUnselectVisitor::Visit(BezierCurveC0& curve)
{
	UniversalUnselect(curve);

	for (auto& segment : curve.m_InstanceData.m_Segments)
	{
		segment.m_Color = BezierCurveC0::UNSELECTED_COLOR;
	}
}

void ObjectUnselectVisitor::UniversalUnselect(SceneObjectCAD& object)
{
	auto& scene = m_SceneDataLayer.m_SceneCAD;
	auto& actionList = m_SceneDataLayer.m_ActionList;
	auto& selectedGroup = m_SceneDataLayer.m_SelectedGroup;

	auto id = object.Id();

	if (!actionList.Selected(id))
	{
		return;
	}

	auto actionToBeApplied = actionList.m_GroupTransform != TransformComponent::IDENTITY;

	auto newAction = actionList.NewAction();
	auto newActionShared = newAction.lock();

	if (actionToBeApplied)
	{
		ApplyAction(newActionShared);
	}

	newActionShared->m_NewlyUnselectedObjects.insert(id);

	auto selectedObjectsCount = newActionShared->SelectedCount();

	if (selectedObjectsCount)
	{
		newActionShared->m_Centroid = ((selectedObjectsCount + 1) * newActionShared->m_Centroid - object.m_Transform.m_Position) / selectedObjectsCount;
	}

	m_SceneDataLayer.m_SceneCAD.m_MainCursor->SetPosition(newActionShared->m_Centroid);

	selectedGroup.Remove(m_Caller);
}
