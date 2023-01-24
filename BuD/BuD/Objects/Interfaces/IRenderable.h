#pragma once

#include <vector>

#include <Renderer/RenderingPass.h>

namespace BuD
{
	class IRenderable
	{
	public:
		IRenderable(std::vector<RenderingPass> renderingPasses)
			: RenderingPasses(renderingPasses)
		{}

		std::vector<RenderingPass> RenderingPasses;
	};
}
