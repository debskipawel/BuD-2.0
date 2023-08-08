#pragma once

#include <map>

#include <Scene/SceneObjectCAD.h>

class CompositeSceneObject
{
public:
	CompositeSceneObject() = default;

	void Add(std::shared_ptr<SceneObjectCAD> object);
	void Remove(std::shared_ptr<SceneObjectCAD> object);
	
	bool Has(std::shared_ptr<SceneObjectCAD> object);

	std::unordered_map<uint32_t, std::shared_ptr<SceneObjectCAD>> m_Objects;
};
