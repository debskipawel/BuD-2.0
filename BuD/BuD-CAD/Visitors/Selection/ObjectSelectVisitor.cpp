#include "ObjectSelectVisitor.h"

#include <Visitors/Transform/ApplyTransformVisitor.h>

ObjectSelectVisitor::ObjectSelectVisitor(SceneDataLayer& sceneDataLayer)
	: BaseObjectSelectionVisitor(sceneDataLayer)
{
}

void ObjectSelectVisitor::Visit(Torus& torus)
{
	UniversalSelect(torus);
}

void ObjectSelectVisitor::Visit(Point& point)
{
	UniversalSelect(point);
}

void ObjectSelectVisitor::UniversalSelect(SceneObjectCAD& object)
{
	auto& scene = m_SceneDataLayer.m_SceneCAD;
	auto& actionList = m_SceneDataLayer.m_ActionList;
	auto& selectedGroup = m_SceneDataLayer.m_SelectedGroup;

	auto id = object.Id();

	if (actionList.Selected(id))
	{
		return;
	}

	auto newAction = actionList.NewAction();
	auto newActionShared = newAction.lock();

	newActionShared->m_NewlySelectedObjects.insert(id);

	if (actionList.m_GroupTransform != TransformComponent::IDENTITY)
	{
		ApplyAction(newActionShared);
	}

	auto selectedObjectsCount = newActionShared->SelectedCount();
	newActionShared->m_Centroid = ((selectedObjectsCount - 1) * newActionShared->m_Centroid + object.m_Transform.m_Position) / selectedObjectsCount;

	m_SceneDataLayer.m_SceneCAD.m_MainCursor->SetPosition(newActionShared->m_Centroid);

	selectedGroup.Add(m_Caller);
}
