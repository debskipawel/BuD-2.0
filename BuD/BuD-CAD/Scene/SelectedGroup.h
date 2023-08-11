#pragma once

#include <set>

#include <Scene/SceneObjectCAD.h>

class SelectedGroup
{
public:
	SelectedGroup() = default;

	void Add(std::weak_ptr<SceneObjectCAD> object);
	void Remove(std::weak_ptr<SceneObjectCAD> object);
	void Clear();

	TransformComponent m_GroupTransform;

	std::set<uint32_t> m_SelectedObjects;
	std::map<uint32_t, TransformComponent> m_InitialTransformCopies;
};
