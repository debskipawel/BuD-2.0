#pragma once

#include <memory>
#include <vector>

#include <Actions/Action.h>

class ActionList
{
public:
	ActionList();

	bool Selected(uint32_t id) const;
	uint32_t SelectedCount() const;

	std::weak_ptr<Action> NewAction();

	std::weak_ptr<Action> Undo();
	std::weak_ptr<Action> Redo();

	std::weak_ptr<Action> Last() const;

	TransformComponent m_GroupTransform;

protected:
	void Add(std::shared_ptr<Action> action);

	std::shared_ptr<Action> InternalLast() const;

	int m_CurrentActionIndex;
	int m_UsableActionListSize;

	std::vector<std::shared_ptr<Action>> m_ActionList;
};
