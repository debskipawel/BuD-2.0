#pragma once

#include <vector>

#include <Renderer/RenderingPass.h>

namespace BuD
{
	class IRenderable
	{
	public:
		virtual std::vector<RenderingPass> RenderingPasses() const { return m_RenderingPasses; };

	protected:
		std::vector<RenderingPass> m_RenderingPasses;
	};
}
