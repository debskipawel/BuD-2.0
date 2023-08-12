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
	return m_ActionList.size() 
		? m_ActionList[m_CurrentActionIndex] 
		: std::weak_ptr<Action>();
}
