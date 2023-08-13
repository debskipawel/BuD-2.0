#include "ActionList.h"

ActionList::ActionList()
	: m_ActionList(), m_CurrentActionIndex(-1), m_UsableActionListSize(0)
{
}

void ActionList::Add(std::shared_ptr<Action> action)
{
	if (m_CurrentActionIndex + 1 < m_ActionList.size())
	{
		m_ActionList[m_CurrentActionIndex++] = action;
		m_UsableActionListSize = m_CurrentActionIndex + 1;
		
		return;
	}

	m_ActionList.push_back(action);
	m_UsableActionListSize = m_ActionList.size();
	m_CurrentActionIndex++;
}

bool ActionList::Selected(uint32_t id) const
{
	auto lastAction = InternalLast();

	return lastAction && lastAction->Selected(id);
}

uint32_t ActionList::SelectedCount() const
{
	auto lastAction = InternalLast();

	return lastAction ? lastAction->SelectedCount() : 0;
}

std::weak_ptr<Action> ActionList::NewAction()
{
	if (m_GroupTransform == TransformComponent::IDENTITY)
	{
		auto selectedCount = SelectedCount();

		if (m_ActionList.size() && SelectedCount())
		{
			return InternalLast();
		}
		else
		{
			auto action = std::make_shared<Action>(TransformComponent::IDENTITY);
			Add(action);

			return action;
		}
	}

	auto lastAction = InternalLast();
	auto action = std::make_shared<Action>(m_GroupTransform);

	auto& previouslySelected = lastAction->m_CurrentlySelectedObjects;
	auto& previouslyNewlySelected = lastAction->m_NewlySelectedObjects;
	auto& previouslyNewlyUnselected = lastAction->m_NewlyUnselectedObjects;

	action->m_CurrentlySelectedObjects = previouslySelected;
	action->m_Centroid = lastAction->m_Centroid + m_GroupTransform.m_Position;

	for (auto& id : previouslyNewlySelected)
	{
		action->m_CurrentlySelectedObjects.insert(id);
	}

	std::erase_if(action->m_CurrentlySelectedObjects, [&previouslyNewlyUnselected](uint32_t objectId) { return previouslyNewlyUnselected.contains(objectId); });

	Add(action);

	m_GroupTransform = TransformComponent::IDENTITY;

	return action;
}

std::weak_ptr<Action> ActionList::Undo()
{
	if (m_CurrentActionIndex > 0)
	{
		return m_ActionList[m_CurrentActionIndex--];
	}

	return std::weak_ptr<Action>();
}

std::weak_ptr<Action> ActionList::Redo()
{
	if (m_CurrentActionIndex + 1 < m_UsableActionListSize)
	{
		return m_ActionList[m_CurrentActionIndex++];
	}

	return std::weak_ptr<Action>();
}

std::weak_ptr<Action> ActionList::Last() const
{
	return InternalLast();
}

std::shared_ptr<Action> ActionList::InternalLast() const
{
	return m_ActionList.size()
		? m_ActionList[m_CurrentActionIndex]
		: std::shared_ptr<Action>();
}
