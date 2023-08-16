#include "TransformActionList.h"

TransformActionList::TransformActionList()
	: m_ActionList(), m_CurrentActionIndex(-1), m_UsableActionListSize(0)
{
}

void TransformActionList::Add(std::shared_ptr<TransformAction> action)
{
	if (m_CurrentActionIndex + 1 < m_ActionList.size())
	{
		m_ActionList[++m_CurrentActionIndex] = action;
		m_UsableActionListSize = m_CurrentActionIndex + 1;

		return;
	}

	m_ActionList.push_back(action);
	m_UsableActionListSize = m_ActionList.size();
	m_CurrentActionIndex++;
}

bool TransformActionList::CanGoBack()
{
	return m_CurrentActionIndex >= 0;
}

bool TransformActionList::CanGoForward()
{
	return m_CurrentActionIndex + 1 < m_UsableActionListSize;
}

std::weak_ptr<TransformAction> TransformActionList::GoBack()
{
	if (CanGoBack())
	{
		return m_ActionList[m_CurrentActionIndex--];
	}

	return std::weak_ptr<TransformAction>();
}

std::weak_ptr<TransformAction> TransformActionList::GoForward()
{
	if (CanGoForward())
	{
		return m_ActionList[++m_CurrentActionIndex];
	}

	return std::weak_ptr<TransformAction>();
}

std::weak_ptr<TransformAction> TransformActionList::Last() const
{
	return InternalLast();
}

std::shared_ptr<TransformAction> TransformActionList::InternalLast() const
{
	return m_ActionList.size()
		? m_ActionList[m_CurrentActionIndex]
		: std::shared_ptr<TransformAction>();
}
