#include "AbstractVisitor.h"

void AbstractVisitor::Visit(std::weak_ptr<SceneObjectCAD> object)
{
	m_Caller = object;

	auto objectShared = object.lock();

	if (objectShared)
	{
		objectShared->Accept(*this);
	}
}
