#include "TransformSelectionSystem.h"

#include <numeric>

#include <Visitors/Transform/UpdateTransformVisitor.h>

void TransformSelectionSystem::Clear()
{
	BaseSelectionSystem::Clear();

	m_InitialTransforms.clear();
	m_TransformActionList = TransformActionList();

	m_GroupTransform = TransformComponent::IDENTITY;

	UpdateCentroid();
}

void TransformSelectionSystem::Select(std::weak_ptr<SceneObjectCAD> sceneObject)
{
	auto sceneObjectShared = sceneObject.lock();

	if (!sceneObjectShared)
	{
		return;
	}
	
	auto action = CreateAction();

	if (!action->m_TransformedObjects.empty())
	{
		m_TransformActionList.Add(action);
	}

	auto selectedCount = Count();

	auto id = sceneObjectShared->Id();

	m_SelectedObjects.emplace(id, sceneObject);
	m_InitialTransforms.emplace(id, sceneObjectShared->m_Transform);

	UpdateCentroid();
}

void TransformSelectionSystem::Unselect(uint32_t sceneObjectId)
{
	if (!m_SelectedObjects.contains(sceneObjectId))
	{
		return;
	}

	auto& object = m_SelectedObjects.at(sceneObjectId);
	auto objectShared = object.lock();

	if (!objectShared)
	{
		return;
	}

	auto action = CreateAction();

	if (!action->m_TransformedObjects.empty())
	{
		m_TransformActionList.Add(action);
	}

	m_SelectedObjects.erase(sceneObjectId);
	m_InitialTransforms.erase(sceneObjectId);

	UpdateCentroid();
}

TransformComponent TransformSelectionSystem::InitialTransform(uint32_t objectId) const
{
	return m_InitialTransforms.contains(objectId) ? m_InitialTransforms.at(objectId) : TransformComponent::IDENTITY;
}

TransformComponent& TransformSelectionSystem::GroupTransform()
{
	return m_GroupTransform;
}

dxm::Vector3 TransformSelectionSystem::Centroid() const
{
	if (m_SelectedObjects.size() == 1)
	{
		auto& [id, object] = *m_SelectedObjects.begin();
		auto objectShared = object.lock();

		return objectShared->m_Transform.m_Position;
	}

	return m_Centroid + m_GroupTransform.m_Position;
}

bool TransformSelectionSystem::CanUndo()
{
	bool flag = m_TransformActionList.CanGoBack();

	if (m_SelectedObjects.size() == 1)
	{
		auto& object = m_SelectedObjects.begin()->second;
		auto objectShared = object.lock();

		flag = flag || (objectShared ? m_InitialTransforms[objectShared->Id()] != objectShared->m_Transform : true);
	}

	if (m_SelectedObjects.size() > 1)
	{
		flag = flag || m_GroupTransform != TransformComponent::IDENTITY;
	}

	return flag;
}

bool TransformSelectionSystem::CanRedo()
{
	bool flag = m_TransformActionList.CanGoForward();

	if (m_SelectedObjects.size() == 1)
	{
		auto& object = m_SelectedObjects.begin()->second;
		auto objectShared = object.lock();

		flag = flag && (objectShared ? m_InitialTransforms[objectShared->Id()] != objectShared->m_Transform : true);
	}

	if (m_SelectedObjects.size() > 1)
	{
		flag = flag && m_GroupTransform == TransformComponent::IDENTITY;
	}

	return flag;
}

bool TransformSelectionSystem::Undo()
{
	auto action = CreateAction();

	if (!action->m_TransformedObjects.empty())
	{
		m_TransformActionList.Add(action);
	}

	auto lastAction = m_TransformActionList.GoBack();
	auto actionShared = lastAction.lock();

	if (actionShared)
	{
		std::unique_ptr<AbstractVisitor> visitor = std::make_unique<UpdateTransformVisitor>();

		for (auto& [id, object] : actionShared->m_TransformedObjects)
		{
			auto objectShared = object.lock();

			if (!objectShared)
			{
				continue;
			}

			auto& initialTransform = actionShared->m_OriginalTransforms[id];

			objectShared->m_Transform = initialTransform;
			visitor->Visit(object);

			m_InitialTransforms[id] = initialTransform;
		}

		UpdateCentroid();

		return true;
	}

	return false;
}

bool TransformSelectionSystem::Redo()
{
	auto groupTransform = m_GroupTransform;
	auto currentAction = CreateAction();

	if (!currentAction->m_TransformedObjects.empty())
	{
		m_GroupTransform = groupTransform;

		return false;
	}
	
	auto action = m_TransformActionList.GoForward();
	auto actionShared = action.lock();

	if (actionShared)
	{
		std::unique_ptr<AbstractVisitor> visitor = std::make_unique<UpdateTransformVisitor>();

		for (auto& [id, object] : actionShared->m_TransformedObjects)
		{
			auto objectShared = object.lock();

			if (!objectShared)
			{
				continue;
			}

			auto& targetTransform = actionShared->m_TargetTransforms[id];

			objectShared->m_Transform = targetTransform;
			visitor->Visit(object);

			m_InitialTransforms[id] = targetTransform;
		}

		UpdateCentroid();

		return true;
	}

	return false;
}

std::shared_ptr<TransformAction> TransformSelectionSystem::CreateAction()
{
	auto action = std::make_shared<TransformAction>();

	for (auto& [id, object] : m_SelectedObjects)
	{
		auto objectShared = object.lock();

		if (!objectShared)
		{
			continue;
		}

		auto& initialTransform = m_InitialTransforms.at(id);
		auto& targetTransform = objectShared->m_Transform;

		if (initialTransform != targetTransform)
		{
			action->m_TransformedObjects.emplace(id, object);
			action->m_OriginalTransforms.emplace(id, initialTransform);
			action->m_TargetTransforms.emplace(id, targetTransform);

			m_InitialTransforms[id] = targetTransform;
		}
	}

	m_GroupTransform = TransformComponent::IDENTITY;

	return action;
}

void TransformSelectionSystem::UpdateCentroid()
{
	auto centroid = dxm::Vector3::Zero;

	for (auto& [id, object] : m_SelectedObjects)
	{
		auto objectShared = object.lock();

		if (!objectShared)
		{
			continue;
		}

		auto& targetTransform = objectShared->m_Transform;

		centroid += targetTransform.m_Position;
	}

	auto selectedCount = Count();

	m_Centroid = selectedCount ? centroid / selectedCount : dxm::Vector3(NAN, NAN, NAN);
}
