#pragma once

#include <d3d11.h>
#include <wrl/client.h>

#include <Scene/Scene.h>

#include <Renderer/GraphicsDevice.h>
#include <Renderer/RenderingPass.h>
#include <Renderer/RenderTarget.h>
#include <Renderer/RendererFrameStats.h>

using namespace Microsoft::WRL;

namespace BuD
{
	class BaseRendererImpl
	{
	public:
		BaseRendererImpl(std::shared_ptr<GraphicsDevice> device) : m_Device(device) {}
		virtual ~BaseRendererImpl() = default;

		virtual dxm::Matrix ProjectionMatrix() = 0;
		virtual RendererFrameStats Render(Scene& scene, const RenderTargetInfo& renderTarget) = 0;

	protected:
		virtual void RenderIndividually(const RenderingPass& renderingPass, Scene& scene);
		virtual void SetupShaderPipeline(const ShaderPipeline& pipeline);
		virtual void CleanAfterDrawing();

		std::shared_ptr<GraphicsDevice> m_Device;
	};
}
