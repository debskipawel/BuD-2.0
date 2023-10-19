#pragma once

#include <map>
#include <memory>
#include <set>

#include <Applications/CAD/Scene/SceneObjectCAD.h>

struct TransformAction
{
public:
	TransformAction()
		: m_TransformedObjects(), m_OriginalTransforms(), m_TargetTransforms()
	{
	}

	std::unordered_map<uint32_t, std::weak_ptr<SceneObjectCAD>> m_TransformedObjects;
	std::unordered_map<uint32_t, TransformComponent> m_OriginalTransforms;
	std::unordered_map<uint32_t, TransformComponent> m_TargetTransforms;
};
