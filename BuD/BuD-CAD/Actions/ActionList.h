#pragma once

#include <memory>
#include <vector>

#include <Actions/Action.h>

class ActionList
{
public:
	ActionList();

	void Add(std::shared_ptr<Action> action);

	std::weak_ptr<Action> Undo();
	std::weak_ptr<Action> Redo();

	std::weak_ptr<Action> Last() const;

protected:
	int m_CurrentActionIndex;
	int m_UsableActionListSize;

	std::vector<std::shared_ptr<Action>> m_ActionList;
};
