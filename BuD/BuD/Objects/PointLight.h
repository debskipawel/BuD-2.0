#pragma once

#include <Scene/SceneEntity.h>

#include "Interfaces/IEmissive.h"
#include "Interfaces/IState.h"
#include "Interfaces/IRenderable.h"

namespace BuD
{
	class PointLight
	{
	public:
		PointLight() = default;
		PointLight(Scene& scene, dxm::Vector3 position, dxm::Vector3 color);

		IState& State() { return m_Entity.GetComponent<IState>(); }
		IEmissive& Emissive() { return m_Entity.GetComponent<IEmissive>(); }

	protected:
		SceneEntity m_Entity;
	};
}
