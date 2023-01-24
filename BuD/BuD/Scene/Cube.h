#pragma once

#include <Renderer/IRenderable.h>

#include "SceneEntity.h"

namespace BuD
{
	class Cube : public SceneEntity, public IRenderable
	{
	public:
		Cube(Scene& scene);
	};
}
