#pragma once

#include "BaseRendererImpl.h"

#include <Renderer/GraphicsDevice.h>

namespace BuD
{
	class StandardRendererImpl : public BaseRendererImpl
	{
	public:
		StandardRendererImpl(std::shared_ptr<GraphicsDevice> device);

		virtual dxm::Matrix ProjectionMatrix() override;
		virtual void Render(Scene& scene, const RenderTargetInfo& renderTarget) override;

	protected:
		std::shared_ptr<GraphicsDevice> m_Device;
		dxm::Matrix m_ProjectionMatrix;

		ComPtr<ID3D11RasterizerState> m_RasterizerState;
	};
}
