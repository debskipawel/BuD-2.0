#include "bud_pch.h"
#include "AnaglyphRendererImpl.h"

#include <Camera/Frustum.h>

#include <Objects/Interfaces/IRenderable.h>
#include <Objects/MeshLoader/MeshLoader.h>
#include <Shaders/Loader/ShaderLoader.h>

#include <Renderer/Renderer.h>
#include <Renderer/Implementations/AnaglyphBlendingShaders.h>
#include <Renderer/Implementations/StandardRendererImpl.h>
#include <Renderer/Structures/ViewportDesc.h>

#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

namespace BuD
{
	constexpr float FOV = 90.0f, NEAR_PLANE = 0.01f, FAR_PLANE = 100.0f;

	AnaglyphRendererImpl::AnaglyphRendererImpl(std::shared_ptr<GraphicsDevice> device)
		: MultiEyeRendererImpl(device), m_PreviousWidth(0), m_PreviousHeight(0)
	{
		m_SingleEyeRendererImpl = std::make_unique<StandardRendererImpl>(device);

		auto meshLoader = MeshLoader();
		m_BlendMeshDetails = meshLoader.LoadPrimitiveMesh(MeshPrimitiveType::QUAD);

		HRESULT hr;

		ComPtr<ID3DBlob> compiledVertexShaderCode, compiledPixelShaderCode;
		ComPtr<ID3DBlob> compilerErrors;
		hr = D3DCompile(g_BlendVertexShader.c_str(), g_BlendVertexShader.size(), NULL, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_5_0", 0, 0, compiledVertexShaderCode.GetAddressOf(), compilerErrors.GetAddressOf());
		hr = D3DCompile(g_BlendPixelShader.c_str(), g_BlendPixelShader.size(), NULL, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_5_0", 0, 0, compiledPixelShaderCode.GetAddressOf(), compilerErrors.GetAddressOf());

		m_BlendShaderPipeline.m_VertexShader = std::make_shared<VertexShader>(m_Device, compiledVertexShaderCode->GetBufferPointer(), compiledVertexShaderCode->GetBufferSize());
		m_BlendShaderPipeline.m_PixelShader = std::make_shared<PixelShader>(m_Device, compiledPixelShaderCode->GetBufferPointer(), compiledPixelShaderCode->GetBufferSize());
	
		D3D11_SAMPLER_DESC samplerDesc;
		ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));

		samplerDesc.AddressU = samplerDesc.AddressV = samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

		hr = m_Device->Device()->CreateSamplerState(&samplerDesc, m_BlendSamplerState.GetAddressOf());
	}
	
	RendererFrameStats AnaglyphRendererImpl::Render(Scene& scene, const RenderTargetInfo& renderTarget)
	{
		m_DrawCallsThisFrame = 0;
		m_InstancesDrawnThisFrame = 0;
		
		float aspectRatio = static_cast<float>(renderTarget.Width) / static_cast<float>(renderTarget.Height);
		UpdateProjectionMatrices(aspectRatio, 90.0f, 0.01f, 100.0f);
		
		UpdateEyeRenderTargets(renderTarget.Width, renderTarget.Height);

		RenderSceneForEye(Eye::LEFT, scene);
		RenderSceneForEye(Eye::RIGHT, scene);

		BlendEyesToSingle(renderTarget);

		RendererFrameStats frameStats{};
		frameStats.m_DrawCalls = m_DrawCallsThisFrame;
		frameStats.m_InstancesDrawn = m_InstancesDrawnThisFrame;
		
		return frameStats;
	}

	void AnaglyphRendererImpl::UpdateEyeRenderTargets(unsigned int width, unsigned int height)
	{
		if (width == m_PreviousWidth && height == m_PreviousHeight)
		{
			return;
		}

		m_EyeRenderTargets[Eye::LEFT] = std::make_unique<RenderTarget>(m_Device, width, height);
		m_EyeRenderTargets[Eye::RIGHT] = std::make_unique<RenderTarget>(m_Device, width, height);

		m_PreviousWidth = width;
		m_PreviousHeight = height;
	}

	void AnaglyphRendererImpl::RenderSceneForEye(Eye eye, Scene& scene)
	{
		m_EyeForCurrentRendering = eye;

		auto& renderTarget = m_EyeRenderTargets[eye];

		if (!renderTarget)
		{
			return;
		}

		auto& context = m_Device->Context();

		float cleanColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
		context->ClearRenderTargetView(renderTarget->RenderTargetView.Get(), cleanColor);
		context->ClearDepthStencilView(renderTarget->DepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		RenderTargetInfo renderTargetInfo{ renderTarget->RenderTargetView, renderTarget->DepthStencilView, renderTarget->Width, renderTarget->Height };

		auto stats = m_SingleEyeRendererImpl->Render(scene, renderTargetInfo);
		
		m_DrawCallsThisFrame += stats.m_DrawCalls;
		m_InstancesDrawnThisFrame += stats.m_InstancesDrawn;
	}

	void AnaglyphRendererImpl::BlendEyesToSingle(const RenderTargetInfo& renderTarget)
	{
		auto& leftEye = m_EyeRenderTargets[Eye::LEFT];
		auto& rightEye = m_EyeRenderTargets[Eye::RIGHT];

		if (!leftEye || !rightEye)
		{
			return;
		}
		
		auto& context = m_Device->Context();

		ViewportDesc viewportDesc{ renderTarget.Width, renderTarget.Height };

		context->OMSetRenderTargets(1, renderTarget.RenderTargetView.GetAddressOf(), renderTarget.DepthStencilView.Get());
		context->RSSetViewports(1, &viewportDesc);

		ID3D11ShaderResourceView* eyeSRV[] = { leftEye->ShaderResourceView.Get(), rightEye->ShaderResourceView.Get() };

		auto& vb = m_BlendMeshDetails.m_VertexBuffer;
		auto& ib = m_BlendMeshDetails.m_IndexBuffer;
		auto& il = m_BlendMeshDetails.m_InputLayout;

		auto& vs = m_BlendShaderPipeline.m_VertexShader;
		auto& ps = m_BlendShaderPipeline.m_PixelShader;

		ID3D11Buffer* vertexBuffers[] = { vb->Get() };
		UINT strides[] = { vb->Stride() };
		UINT offsets[] = { vb->Offset() };

		context->IASetInputLayout(il->Layout());
		context->IASetVertexBuffers(0, 1, vertexBuffers, strides, offsets);
		context->IASetIndexBuffer(ib->Get(), ib->Format(), 0);
		context->IASetPrimitiveTopology(ib->Topology());

		context->VSSetShader(vs->Shader(), nullptr, 0);
		context->PSSetShader(ps->Shader(), nullptr, 0);

		ID3D11SamplerState* samplers[] = { m_BlendSamplerState.Get() };

		context->PSSetSamplers(0, 1, samplers);
		context->PSSetShaderResources(0, 2, eyeSRV);

		context->DrawIndexed(ib->Count(), 0, 0);
		
		m_DrawCallsThisFrame++;

		CleanAfterDrawing();
	}
}
