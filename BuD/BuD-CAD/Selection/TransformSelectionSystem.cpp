#include "TransformSelectionSystem.h"

#include <numeric>

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

std::shared_ptr<TransformAction> TransformSelectionSystem::Undo()
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
		for (auto& [id, initialTransform] : actionShared->m_OriginalTransforms)
		{
			m_InitialTransforms[id] = initialTransform;
		}

		UpdateCentroid();

		return actionShared;
	}

	return std::shared_ptr<TransformAction>();
}

std::shared_ptr<TransformAction> TransformSelectionSystem::Redo()
{
	auto groupTransform = m_GroupTransform;
	auto currentAction = CreateAction();

	if (!currentAction->m_TransformedObjects.empty())
	{
		m_GroupTransform = groupTransform;

		return std::shared_ptr<TransformAction>();;
	}
	
	auto action = m_TransformActionList.GoForward();
	auto actionShared = action.lock();

	if (actionShared)
	{
		for (auto& [id, targetTransform] : actionShared->m_TargetTransforms)
		{
			m_InitialTransforms[id] = targetTransform;
		}

		UpdateCentroid();

		return actionShared;
	}

	return std::shared_ptr<TransformAction>();
}

std::shared_ptr<TransformAction> TransformSelectionSystem::CreateAction()
{
	auto action = std::make_shared<TransformAction>();

	m_Centroid = dxm::Vector3::Zero;

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
			m_Centroid += targetTransform.m_Position;

			action->m_TransformedObjects.emplace(id, object);
			action->m_OriginalTransforms.emplace(id, initialTransform);
			action->m_TargetTransforms.emplace(id, targetTransform);

			m_InitialTransforms[id] = targetTransform;
		}
	}

	auto selectedCount = Count();

	if (selectedCount)
	{
		m_Centroid /= selectedCount;
	}

	m_GroupTransform = TransformComponent::IDENTITY;

	return action;
}

void TransformSelectionSystem::UpdateCentroid()
{
	m_Centroid = dxm::Vector3::Zero;

	for (auto& [id, object] : m_SelectedObjects)
	{
		auto objectShared = object.lock();

		if (!objectShared)
		{
			continue;
		}

		auto& targetTransform = objectShared->m_Transform;

		m_Centroid += targetTransform.m_Position;
	}

	auto selectedCount = Count();

	if (selectedCount)
	{
		m_Centroid /= selectedCount;
	}
}
