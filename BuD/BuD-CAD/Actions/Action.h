#pragma once

#include <map>
#include <set>

#include <Components/TransformComponent.h>

struct Action
{
	Action(const TransformComponent& transform)
		: m_TransformComponent(transform), m_CurrentlySelectedObjects(), m_NewlySelectedObjects(), m_NewlyUnselectedObjects()
	{
	}

	uint32_t SelectedCount() const;
	bool Selected(uint32_t id) const;

	std::set<uint32_t> m_CurrentlySelectedObjects;
	std::set<uint32_t> m_NewlySelectedObjects;
	std::set<uint32_t> m_NewlyUnselectedObjects;

	TransformComponent m_TransformComponent;
	dxm::Vector3 m_Centroid;
};
