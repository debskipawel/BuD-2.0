#include "CompositeSceneObject.h"

void CompositeSceneObject::Add(std::weak_ptr<SceneObjectCAD> object)
{
	auto objectShared = object.lock();

	if (m_Objects.find(objectShared->Id()) != m_Objects.end())
	{
		return;
	}

	m_Objects.emplace(objectShared->Id(), object);
}

void CompositeSceneObject::Remove(uint32_t objectId)
{
	m_Objects.erase(objectId);
}

bool CompositeSceneObject::Has(uint32_t objectId)
{
	return m_Objects.find(objectId) != m_Objects.end();
}
