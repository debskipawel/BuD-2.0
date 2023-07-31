#include "bud_pch.h"
#include "BaseRendererImpl.h"

#include <Utils/Log.h>

namespace BuD
{
	void BaseRendererImpl::RenderIndividually(const RenderingPass& renderingPass, Scene& scene)
	{
		auto& pipeline = renderingPass.m_Pipeline;
		auto& mesh = renderingPass.m_Mesh;

		auto& vb = mesh.m_VertexBuffer;
		auto& ib = mesh.m_IndexBuffer;

		auto& il = mesh.m_InputLayout;

		renderingPass.m_PreRenderCallback(renderingPass, scene);

		auto rawVertexBuffer = vb->Get();
		const auto stride = vb->Stride();
		const auto offset = vb->Offset();

		auto& context = m_Device->Context();

		context->IASetInputLayout(il->Layout());
		context->IASetVertexBuffers(0, 1, &rawVertexBuffer, (const UINT*)(&stride), (const UINT*)(&offset));
		context->IASetIndexBuffer(ib->Get(), ib->Format(), 0);
		context->IASetPrimitiveTopology(ib->Topology());

		SetupShaderPipeline(pipeline);

		context->DrawIndexed(ib->Count(), 0, 0);

		CleanAfterDrawing();
	}

	void BaseRendererImpl::SetupShaderPipeline(const ShaderPipeline& pipeline)
	{
		auto& vs = pipeline.m_VertexShader;
		auto& hs = pipeline.m_HullShader;
		auto& ds = pipeline.m_DomainShader;
		auto& gs = pipeline.m_GeometryShader;
		auto& ps = pipeline.m_PixelShader;

		auto& context = m_Device->Context();

		auto vsCB = vs->RawConstantBuffers();
		context->VSSetConstantBuffers(0, vsCB.size(), vsCB.data());
		context->VSSetShader(vs->Shader(), nullptr, 0);

		if (gs)
		{
			auto gsCB = gs->RawConstantBuffers();
			context->GSSetConstantBuffers(0, gsCB.size(), gsCB.data());
			context->GSSetShader(gs->Shader(), nullptr, 0);
		}

		if ((hs && !ds) || (!hs && ds))
		{
			Log::WriteError(L"Trying to use tessellation without using either hull and domain shader.");
		}

		if (hs && ds)
		{
			auto hsCB = hs->RawConstantBuffers();
			auto dsCB = ds->RawConstantBuffers();

			context->HSSetConstantBuffers(0, hsCB.size(), hsCB.data());
			context->HSSetShader(hs->Shader(), nullptr, 0);

			context->DSSetConstantBuffers(0, dsCB.size(), dsCB.data());
			context->DSSetShader(ds->Shader(), nullptr, 0);
		}

		auto psCB = ps->RawConstantBuffers();
		context->PSSetConstantBuffers(0, psCB.size(), psCB.data());
		context->PSSetShader(ps->Shader(), nullptr, 0);
	}

	void BaseRendererImpl::CleanAfterDrawing()
	{
		auto& context = m_Device->Context();

		ID3D11ShaderResourceView* clean[] = { nullptr };

		context->VSSetShaderResources(0, 1, clean);
		context->GSSetShaderResources(0, 1, clean);
		context->HSSetShaderResources(0, 1, clean);
		context->DSSetShaderResources(0, 1, clean);
		context->PSSetShaderResources(0, 1, clean);

		context->GSSetShader(nullptr, nullptr, 0);
		context->HSSetShader(nullptr, nullptr, 0);
		context->DSSetShader(nullptr, nullptr, 0);
	}
}
