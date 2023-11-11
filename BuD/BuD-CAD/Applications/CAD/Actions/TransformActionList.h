#pragma once

#include <Applications/CAD/Actions/TransformAction.h>

class TransformActionList
{
public:
	TransformActionList();

	void Add(std::shared_ptr<TransformAction> action);

	bool CanGoBack();
	bool CanGoForward();

	std::weak_ptr<TransformAction> GoBack();
	std::weak_ptr<TransformAction> GoForward();

	std::weak_ptr<TransformAction> Last() const;

protected:
	std::shared_ptr<TransformAction> InternalLast() const;

	int m_CurrentActionIndex;
	int m_UsableActionListSize;

	std::vector<std::shared_ptr<TransformAction>> m_ActionList;
};
