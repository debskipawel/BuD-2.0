#include "bud_pch.h"
#include "BaseRendererImpl.h"

#include <Renderer/Structures/RasterizerDesc.h>
#include <Utils/Log.h>

namespace BuD
{
	void BaseRendererImpl::UseLeftHandCoordinateSystem()
	{
		m_UseRightHandCoordinateSystem = false;
	}

	void BaseRendererImpl::UseRightHandCoordinateSystem()
	{
		m_UseRightHandCoordinateSystem = true;
	}

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

		SetupRasterizerState(renderingPass.m_RasterizerDescription);
		SetupShaderPipeline(pipeline);

		context->DrawIndexed(ib->Count(), 0, 0);

		CleanAfterDrawing();
	}

	void BaseRendererImpl::SetupRasterizerState(RasterizerDescription rasterizerDescription)
	{
		auto& context = m_Device->Context();
		auto result = m_RasterizerStates.find(rasterizerDescription);

		if (result != m_RasterizerStates.end())
		{
			auto rastState = result->second.Get();

			context->RSSetState(rastState);
			return;
		}

		auto device = m_Device->Device();
		
		RasterizerDesc rasterizerDesc;
		rasterizerDesc.CullMode = s_CullModeMap.at(rasterizerDescription.m_CullType);
		rasterizerDesc.FillMode = s_FillModeMap.at(rasterizerDescription.m_FillMode);

		ComPtr<ID3D11RasterizerState> state;
		auto hr = device->CreateRasterizerState(&rasterizerDesc, state.GetAddressOf());

		context->RSSetState(state.Get());

		m_RasterizerStates.emplace(rasterizerDescription, state);
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
			Log::WriteError("Trying to use tessellation without using either hull and domain shader.");
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

		ID3D11ShaderResourceView* clean[] = { nullptr, nullptr };

		context->VSSetShaderResources(0, 2, clean);
		context->GSSetShaderResources(0, 2, clean);
		context->HSSetShaderResources(0, 2, clean);
		context->DSSetShaderResources(0, 2, clean);
		context->PSSetShaderResources(0, 2, clean);

		context->GSSetShader(nullptr, nullptr, 0);
		context->HSSetShader(nullptr, nullptr, 0);
		context->DSSetShader(nullptr, nullptr, 0);
	}

	std::unordered_map<CullType, D3D11_CULL_MODE> BaseRendererImpl::s_CullModeMap =
	{
		{ CullType::BACK, D3D11_CULL_BACK },
		{ CullType::FRONT, D3D11_CULL_FRONT },
		{ CullType::NONE, D3D11_CULL_NONE },
	};

	std::unordered_map<FillMode, D3D11_FILL_MODE> BaseRendererImpl::s_FillModeMap =
	{
		{ FillMode::SOLID, D3D11_FILL_SOLID },
		{ FillMode::WIREFRAME, D3D11_FILL_WIREFRAME },
	};
}
