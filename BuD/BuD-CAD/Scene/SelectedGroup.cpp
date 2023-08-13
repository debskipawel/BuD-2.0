#include "SelectedGroup.h"

void SelectedGroup::Add(std::weak_ptr<SceneObjectCAD> object)
{
	auto objectShared = object.lock();
	auto id = objectShared->Id();

	m_SelectedObjects.insert(id);
	m_InitialTransformCopies.emplace(id, objectShared->m_Transform);
}

void SelectedGroup::Remove(std::weak_ptr<SceneObjectCAD> object)
{
	auto objectShared = object.lock();
	auto id = objectShared->Id();

	m_SelectedObjects.erase(id);
	m_InitialTransformCopies.erase(id);
}
