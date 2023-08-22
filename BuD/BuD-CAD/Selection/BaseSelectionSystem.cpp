#include "BaseSelectionSystem.h"

void BaseSelectionSystem::Clear()
{
	m_SelectedObjects.clear();
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
}

void BaseSelectionSystem::Unselect(uint32_t sceneObjectId)
{
	if (!m_SelectedObjects.contains(sceneObjectId))
	{
		return;
	}

	auto& object = m_SelectedObjects.at(sceneObjectId);
	auto objectShared = object.lock();

	if (!objectShared)
	{
		return;
	}

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
	for (auto& [id, object] : m_SelectedObjects)
	{
		auto objectShared = object.lock();

		if (!objectShared)
		{
			continue;
		}

		forEachHandler(objectShared);
	}
}
