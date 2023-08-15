#include "ObjectSelectVisitor.h"

#include <Visitors/Transform/ApplyTransformVisitor.h>

ObjectSelectVisitor::ObjectSelectVisitor(SceneDataLayer& sceneDataLayer)
	: BaseObjectSelectionVisitor(sceneDataLayer)
{
}

void ObjectSelectVisitor::Visit(Torus& torus)
{
	UniversalSelect(torus);

	torus.m_InstanceData.m_Color = Torus::SELECTED_COLOR;
}

void ObjectSelectVisitor::Visit(Point& point)
{
	UniversalSelect(point);

	point.m_InstanceData.m_Color = Point::SELECTED_COLOR;
}

void ObjectSelectVisitor::Visit(BezierCurveC0& curve)
{
	UniversalSelect(curve);

	for (auto& segment : curve.m_InstanceData.m_Segments)
	{
		segment.m_Color = BezierCurveC0::SELECTED_COLOR;
	}
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

	auto actionToBeApplied = actionList.m_GroupTransform != TransformComponent::IDENTITY;

	auto newAction = actionList.NewAction();
	auto newActionShared = newAction.lock();

	if (actionToBeApplied)
	{
		ApplyAction(newActionShared);
	}

	newActionShared->m_NewlySelectedObjects.insert(id);

	auto selectedObjectsCount = newActionShared->SelectedCount();
	newActionShared->m_Centroid = ((selectedObjectsCount - 1) * newActionShared->m_Centroid + object.m_Transform.m_Position) / selectedObjectsCount;

	m_SceneDataLayer.m_SceneCAD.m_MainCursor->SetPosition(newActionShared->m_Centroid);

	selectedGroup.Add(m_Caller);
}
