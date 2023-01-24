#pragma once

#include <Objects/Interfaces/IRenderable.h>
#include <Objects/Interfaces/IState.h>

#include "SceneEntity.h"

namespace BuD
{
	class Cube : public SceneEntity, public IRenderable, public IState
	{
	public:
		Cube(Scene& scene);
	};
}
