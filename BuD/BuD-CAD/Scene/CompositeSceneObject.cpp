#include "CompositeSceneObject.h"

void CompositeSceneObject::Add(std::weak_ptr<SceneObjectCAD> object)
{
	auto objectShared = object.lock();

	if (m_Objects.find(objectShared->Id()) != m_Objects.end())
	{
		return;
	}

	objectShared->OnSelect();
	m_Objects.emplace(objectShared->Id(), object);
}

void CompositeSceneObject::Remove(uint32_t objectId)
{
	auto result = m_Objects.find(objectId);

	if (result == m_Objects.end())
	{
		return;
	}

	// TODO: maybe unselect visitor could be more flexible?
	auto sharedObject = result->second.lock();
	sharedObject->OnUnselect();

	m_Objects.erase(objectId);
}

void CompositeSceneObject::Clear()
{
	for (auto& [id, object] : m_Objects)
	{
		auto sharedObject = object.lock();
		sharedObject->OnUnselect();
	}

	m_Objects.clear();
}

bool CompositeSceneObject::Has(uint32_t objectId)
{
	return m_Objects.find(objectId) != m_Objects.end();
}
