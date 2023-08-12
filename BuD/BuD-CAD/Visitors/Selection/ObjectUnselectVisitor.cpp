#include "ObjectUnselectVisitor.h"

#include <Visitors/Transform/ApplyTransformVisitor.h>

ObjectUnselectVisitor::ObjectUnselectVisitor(SceneDataLayer& sceneDataLayer)
	: m_SceneDataLayer(sceneDataLayer)
{
}

void ObjectUnselectVisitor::Visit(Torus& torus)
{
	UniversalUnselect(torus);
}

void ObjectUnselectVisitor::Visit(Point& point)
{
	UniversalUnselect(point);
}

void ObjectUnselectVisitor::UniversalUnselect(SceneObjectCAD& object)
{
	auto& scene = m_SceneDataLayer.m_SceneCAD;
	auto& actionList = m_SceneDataLayer.m_ActionList;
	auto& selectedGroup = m_SceneDataLayer.m_SelectedGroup;

	auto id = object.Id();

	auto& groupTransform = selectedGroup.m_GroupTransform;

	auto lastAction = actionList.Last();
	auto lastActionShared = lastAction.lock();

	// No transformation was performed since last select/unselect
	// Just need to add newly selected object to the previous action
	// (or create a new action if no other object was previously selected)
	if (groupTransform == TransformComponent::IDENTITY)
	{
		lastActionShared->m_NewlyUnselectedObjects.insert(id);
		selectedGroup.Remove(m_Caller);

		auto selectedObjectsCount = lastActionShared->m_CurrentlySelectedObjects.size() + lastActionShared->m_NewlySelectedObjects.size() - lastActionShared->m_NewlyUnselectedObjects.size();
		
		if (selectedObjectsCount)
		{
			lastActionShared->m_Centroid = ((selectedObjectsCount + 1) * lastActionShared->m_Centroid - object.m_Transform.m_Position) / selectedObjectsCount;

			m_SceneDataLayer.m_SceneCAD.m_MainCursor->SetPosition(lastActionShared->m_Centroid);
		}

		return;
	}

	auto action = std::make_shared<Action>(groupTransform);

	// To future me: YES - it should be here. If we got here, then it means that some transformation has been made.
	auto& previouslySelected = lastActionShared->m_CurrentlySelectedObjects;
	auto& previouslyNewlySelected = lastActionShared->m_NewlySelectedObjects;
	auto& previouslyNewlyUnselected = lastActionShared->m_NewlyUnselectedObjects;

	action->m_CurrentlySelectedObjects = previouslySelected;

	for (auto& id : previouslyNewlySelected)
	{
		action->m_CurrentlySelectedObjects.insert(id);
	}

	std::erase_if(action->m_CurrentlySelectedObjects, [&previouslyNewlyUnselected](uint32_t objectId) { return previouslyNewlyUnselected.contains(objectId); });

	action->m_NewlyUnselectedObjects.insert(id);
	action->m_Centroid = lastActionShared->m_Centroid + groupTransform.m_Position;

	m_SceneDataLayer.m_SceneCAD.m_MainCursor->SetPosition(action->m_Centroid);

	actionList.Add(action);

	for (auto& id : action->m_CurrentlySelectedObjects)
	{
		auto& originalTransform = selectedGroup.m_InitialTransformCopies[id];
		auto& object = scene.m_ObjectList[id];

		std::unique_ptr<AbstractVisitor> visitor = std::make_unique<ApplyTransformVisitor>(originalTransform, groupTransform, lastActionShared->m_Centroid);
		visitor->Visit(object);

		selectedGroup.m_InitialTransformCopies[id] = object->m_Transform;
	}

	selectedGroup.m_GroupTransform = TransformComponent::IDENTITY;

	selectedGroup.Remove(m_Caller);
}
