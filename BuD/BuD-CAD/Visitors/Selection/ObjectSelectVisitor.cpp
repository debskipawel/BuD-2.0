#include "ObjectSelectVisitor.h"

#include <Visitors/Transform/ApplyTransformVisitor.h>

ObjectSelectVisitor::ObjectSelectVisitor(SceneDataLayer& sceneDataLayer)
	: m_SceneDataLayer(sceneDataLayer)
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

	auto& groupTransform = selectedGroup.m_GroupTransform;

	auto lastAction = actionList.Last();
	auto lastActionShared = lastAction.lock();

	// No transformation was performed since last select/unselect
	// Just need to add newly selected object to the previous action
	// (or create a new action if no other object was previously selected)
	if (groupTransform == TransformComponent::IDENTITY)
	{
		if (lastActionShared && selectedGroup.m_SelectedObjects.size())
		{
			lastActionShared->m_NewlySelectedObjects.insert(id);
			
			auto selectedObjectsCount = lastActionShared->m_CurrentlySelectedObjects.size() + lastActionShared->m_NewlySelectedObjects.size() - lastActionShared->m_NewlyUnselectedObjects.size();
			lastActionShared->m_Centroid = ((selectedObjectsCount - 1) * lastActionShared->m_Centroid + object.m_Transform.m_Position) / selectedObjectsCount;

			m_SceneDataLayer.m_SceneCAD.m_MainCursor->SetPosition(lastActionShared->m_Centroid);
		}
		else
		{
			auto action = std::make_shared<Action>(TransformComponent::IDENTITY);

			action->m_NewlySelectedObjects.insert(id);
			action->m_TransformComponent = object.m_Transform;
			action->m_Centroid = object.m_Transform.m_Position;

			m_SceneDataLayer.m_SceneCAD.m_MainCursor->SetPosition(action->m_Centroid);

			actionList.Add(action);
		}

		selectedGroup.Add(m_Caller);

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

	action->m_NewlySelectedObjects.insert(id);
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

	selectedGroup.Add(m_Caller);
}
