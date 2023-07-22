#include "bud_pch.h"
#include "StandardRendererImpl.h"

#include <Objects/Interfaces/IRenderable.h>
#include <Renderer/Structures/ViewportDesc.h>

namespace BuD
{
	StandardRendererImpl::StandardRendererImpl(std::shared_ptr<GraphicsDevice> device)
		: m_Device(device)
	{
		D3D11_RASTERIZER_DESC rsDesc{};
		rsDesc.CullMode = D3D11_CULL_BACK;
		rsDesc.FillMode = D3D11_FILL_SOLID;
		rsDesc.FrontCounterClockwise = false;

		device->Device()->CreateRasterizerState(&rsDesc, m_RasterizerState.GetAddressOf());
	}

	dxm::Matrix StandardRendererImpl::ProjectionMatrix()
	{
		return m_ProjectionMatrix;
	}

	void StandardRendererImpl::Render(Scene& scene, const RenderTargetInfo& renderTarget)
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
		context->RSSetState(m_RasterizerState.Get());

		for (auto [entity, renderable] : scene.GetAllEntitiesWith<IRenderable>())
		{
			auto id = entity;
			SceneEntity entity(scene, id);

			for (auto& renderingPass : renderable.RenderingPasses)
			{
				auto& pipeline = renderingPass.m_Pipeline;
				auto& mesh = renderingPass.m_Mesh;

				auto& vb = mesh.m_VertexBuffer;
				auto& ib = mesh.m_IndexBuffer;

				auto& il = mesh.m_InputLayout;

				auto& vs = pipeline.m_VertexShader;
				auto& hs = pipeline.m_HullShader;
				auto& ds = pipeline.m_DomainShader;
				auto& gs = pipeline.m_GeometryShader;
				auto& ps = pipeline.m_PixelShader;

				if (!vb || !ib || !vs || !ps)
				{
					Log::WriteWarning(L"Attempting to render without necessary assets (vertex & index buffers, vertex & pixel shaders). Abandoning.");
					continue;
				}

				renderingPass.m_PreRenderCallback(renderingPass, entity);

				auto rawVertexBuffer = vb->Get();
				const auto stride = vb->Stride();
				const auto offset = vb->Offset();

				context->IASetInputLayout(il->Layout());
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

				ID3D11ShaderResourceView* clean[] = { nullptr };

				context->VSSetShaderResources(0, 1, clean);
				context->PSSetShaderResources(9, 1, clean);
			}
		}
	}
}
