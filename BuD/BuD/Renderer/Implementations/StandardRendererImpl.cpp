#include "bud_pch.h"
#include "StandardRendererImpl.h"

#include <Objects/Interfaces/IRenderable.h>
#include <Renderer/Structures/ViewportDesc.h>

namespace BuD
{
	StandardRendererImpl::StandardRendererImpl(std::shared_ptr<GraphicsDevice> device)
		: m_Device(device)
	{
	}

	dxm::Matrix StandardRendererImpl::ProjectionMatrix()
	{
		return m_ProjectionMatrix;
	}

	void StandardRendererImpl::Render(const Scene& scene, const RenderTargetInfo& renderTarget)
	{
		m_ProjectionMatrix = dxm::Matrix::CreatePerspectiveFieldOfView(
			DirectX::XMConvertToRadians(90.0f), 
			static_cast<float>(renderTarget.Width) / renderTarget.Height, 
			0.01f, 100.0f
		);

		auto& context = m_Device->Context();

		ViewportDesc viewportDesc{ renderTarget.Width, renderTarget.Height };

		context->OMSetRenderTargets(1, renderTarget.RenderTargetView.GetAddressOf(), renderTarget.DepthStencilView.Get());
		context->RSSetViewports(1, &viewportDesc);

		for (auto& [uuid, sceneEntity] : scene.m_SceneEntities)
		{
			if (!sceneEntity->Implements<IRenderable>())
			{
				continue;
			}

			auto renderable = sceneEntity->Component<IRenderable>();

			for (auto& renderingPass : renderable->RenderingPasses())
			{
				auto& vb = renderingPass.VertexBuffer;
				auto& ib = renderingPass.IndexBuffer;

				auto& vs = renderingPass.VertexShader;
				auto& hs = renderingPass.HullShader;
				auto& ds = renderingPass.DomainShader;
				auto& gs = renderingPass.GeometryShader;
				auto& ps = renderingPass.PixelShader;

				if (!vb || !ib || !vs || !ps)
				{
					Log::WriteWarning(L"Attempting to render without necessary assets (vertex & index buffers, vertex & pixel shaders). Abandoning.");
					continue;
				}

				renderingPass.PreRenderCallback(renderingPass);

				auto rawVertexBuffer = vb->Get();
				const auto stride = vb->Stride();
				const auto offset = vb->Offset();

				context->IASetInputLayout(vs->GetLayout());
				context->IASetVertexBuffers(0, 1, &rawVertexBuffer, (const UINT*)(&stride), (const UINT*)(&offset));
				context->IASetIndexBuffer(ib->Get(), ib->Format(), 0);
				context->IASetPrimitiveTopology(ib->Topology());

				auto vsCB = vs->RawConstantBuffers();
				context->VSSetConstantBuffers(0, vsCB.size(), vsCB.data());
				context->VSSetShader(vs->Shader(), nullptr, 0);

				auto psCB = ps->RawConstantBuffers();
				context->PSSetConstantBuffers(0, psCB.size(), psCB.data());
				context->PSSetShader(ps->Shader(), nullptr, 0);

				context->DrawIndexed(ib->Count(), 0, 0);
			}
		}
	}
}
