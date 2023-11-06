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
		BaseRendererImpl(std::shared_ptr<GraphicsDevice> device) 
			: m_Device(device), m_UseRightHandCoordinateSystem(true)
		{}
		
		virtual ~BaseRendererImpl() = default;

		virtual void UseLeftHandCoordinateSystem();
		virtual void UseRightHandCoordinateSystem();

		virtual dxm::Matrix ProjectionMatrix() = 0;
		virtual RendererFrameStats Render(Scene& scene, const RenderTargetInfo& renderTarget) = 0;

	protected:
		virtual void RenderIndividually(const RenderingPass& renderingPass, Scene& scene);
		virtual void SetupRasterizerState(RasterizerDescription rasterizerDescription);
		virtual void SetupShaderPipeline(const ShaderPipeline& pipeline);
		virtual void CleanAfterDrawing();

		std::map<RasterizerDescription, ComPtr<ID3D11RasterizerState>> m_RasterizerStates;

		std::shared_ptr<GraphicsDevice> m_Device;

		bool m_UseRightHandCoordinateSystem;

		static std::unordered_map<CullType, D3D11_CULL_MODE> s_CullModeMap;
		static std::unordered_map<FillMode, D3D11_FILL_MODE> s_FillModeMap;
	};
}
