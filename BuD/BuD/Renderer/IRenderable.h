#pragma once

#include <vector>

#include "RenderingPass.h"

namespace BuD
{
	class IRenderable
	{
	public:
		virtual std::vector<RenderingPass> RenderingPasses() = 0;
	};
}
