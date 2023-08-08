#include "CompositeSceneObject.h"

void CompositeSceneObject::Add(std::shared_ptr<SceneObjectCAD> object)
{
	if (m_Objects.find(object->Id()) != m_Objects.end())
	{
		return;
	}

	m_Objects.insert({ object->Id(), object});
}

void CompositeSceneObject::Remove(std::shared_ptr<SceneObjectCAD> object)
{
	m_Objects.erase(object->Id());
}

bool CompositeSceneObject::Has(std::shared_ptr<SceneObjectCAD> object)
{
	auto id = object->Id();

	return m_Objects.find(id) != m_Objects.end();
}
