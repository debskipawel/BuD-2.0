#pragma once

#include <map>

#include <Scene/SceneObjectCAD.h>

class CompositeSceneObject
{
public:
	CompositeSceneObject() = default;

	void Add(std::weak_ptr<SceneObjectCAD> object);
	void Remove(uint32_t objectId);
	
	bool Has(uint32_t objectId);

	std::unordered_map<uint32_t, std::weak_ptr<SceneObjectCAD>> m_Objects;
};
