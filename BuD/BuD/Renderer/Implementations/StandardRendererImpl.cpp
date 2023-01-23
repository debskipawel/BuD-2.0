#include "bud_pch.h"
#include "StandardRendererImpl.h"

#include <Renderer/Structures/ViewportDesc.h>

namespace BuD
{
	StandardRendererImpl::StandardRendererImpl(std::shared_ptr<GraphicsDevice> device)
		: m_Device(device)
	{
	}

	void StandardRendererImpl::Render(const Scene& scene, const RenderTargetInfo& renderTarget)
	{
		auto& context = m_Device->Context();

		ViewportDesc viewportDesc{ renderTarget.Width, renderTarget.Height };

		context->OMSetRenderTargets(1, renderTarget.RenderTargetView.GetAddressOf(), renderTarget.DepthStencilView.Get());
		context->RSSetViewports(1, &viewportDesc);

		for (auto& [uuid, sceneEntity] : scene.m_SceneEntities)
		{
		}
	}
}
