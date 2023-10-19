#include "BaseSelectionSystem.h"

void BaseSelectionSystem::Clear()
{
	m_SelectedObjects.clear();
	m_SelectedOrder.clear();
}

void BaseSelectionSystem::Select(std::weak_ptr<SceneObjectCAD> sceneObject)
{
	auto sceneObjectShared = sceneObject.lock();

	if (!sceneObjectShared)
	{
		return;
	}

	auto id = sceneObjectShared->Id();

	m_SelectedObjects.emplace(id, sceneObject);
	m_SelectedOrder.push_back(id);
}

void BaseSelectionSystem::Unselect(uint32_t sceneObjectId)
{
	if (!m_SelectedObjects.contains(sceneObjectId))
	{
		return;
	}

	std::erase_if(m_SelectedOrder, [sceneObjectId](uint32_t id) { return id == sceneObjectId; });
	m_SelectedObjects.erase(sceneObjectId);
}

void BaseSelectionSystem::Unselect(std::weak_ptr<SceneObjectCAD> sceneObject)
{
	auto sceneObjectShared = sceneObject.lock();

	if (!sceneObjectShared)
	{
		return;
	}

	Unselect(sceneObjectShared->Id());
}

void BaseSelectionSystem::ForEachSelected(std::function<void(std::shared_ptr<SceneObjectCAD>)> forEachHandler)
{
	for (auto& id : m_SelectedOrder)
	{
		auto& object = m_SelectedObjects.at(id);

		auto objectShared = object.lock();

		if (!objectShared)
		{
			continue;
		}

		forEachHandler(objectShared);
	}
}
