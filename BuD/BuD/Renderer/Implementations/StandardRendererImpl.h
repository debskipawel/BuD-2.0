#pragma once

#include "BaseRendererImpl.h"

#include <Renderer/GraphicsDevice.h>

namespace BuD
{
	class StandardRendererImpl : public BaseRendererImpl
	{
	public:
		StandardRendererImpl(std::shared_ptr<GraphicsDevice> device);

		virtual void Render(const Scene& scene, const RenderTargetInfo& renderTarget) override;

	protected:
		std::shared_ptr<GraphicsDevice> m_Device;
	};
}
