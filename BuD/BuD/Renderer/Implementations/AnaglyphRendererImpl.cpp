#include "bud_pch.h"
#include "AnaglyphRendererImpl.h"

#include <Camera/Frustum.h>

#include <Objects/Interfaces/IRenderable.h>
#include <Objects/MeshLoader/MeshLoader.h>
#include <Shaders/Loader/ShaderLoader.h>

#include <Renderer/Renderer.h>
#include <Renderer/Implementations/AnaglyphBlendingShaders.h>
#include <Renderer/Structures/ViewportDesc.h>

#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

namespace BuD
{
	constexpr float FOV = 90.0f, NEAR_PLANE = 0.01f, FAR_PLANE = 100.0f;

	AnaglyphRendererImpl::AnaglyphRendererImpl(std::shared_ptr<GraphicsDevice> device)
		: BaseRendererImpl(device), m_PreviousWidth(0), m_PreviousHeight(0)
	{
		m_InstanceBuffer = std::make_shared<InstanceBuffer>(64, nullptr);

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

	dxm::Matrix AnaglyphRendererImpl::ProjectionMatrix()
	{
		return m_ProjectionMatricesForEyes.at(m_EyeForCurrentRendering);
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
	
	void AnaglyphRendererImpl::UpdateProjectionMatrices(float aspectRatio, float fov, float nearPlane, float farPlane)
	{
		float focusPlane = m_AnaglyphSettings.m_FocusPlane;
		float eyeDistance = m_AnaglyphSettings.m_EyeDistance;
		float halfEyeDistance = eyeDistance * 0.5f;

		float top = focusPlane * tanf(DirectX::XMConvertToRadians(fov * 0.5f));
		float bottom = focusPlane * tanf(-DirectX::XMConvertToRadians(fov * 0.5f));
		
		float width = (top - bottom) * aspectRatio;

		float L = -0.5f * width;
		float R = 0.5f * width;

		float leftEyeL = (L + halfEyeDistance) * nearPlane / focusPlane;
		float leftEyeR = (R + halfEyeDistance) * nearPlane / focusPlane;

		float rightEyeL = (L - halfEyeDistance) * nearPlane / focusPlane;
		float rightEyeR = (R - halfEyeDistance) * nearPlane / focusPlane;

		top = top * nearPlane / focusPlane;
		bottom = bottom * nearPlane / focusPlane;

		m_ProjectionMatricesForEyes[Eye::LEFT] = dxm::Matrix::CreateTranslation({ halfEyeDistance, 0.0f, 0.0f })
			* dxm::Matrix::CreatePerspectiveOffCenter(leftEyeL, leftEyeR, bottom, top, nearPlane, farPlane);

		m_ProjectionMatricesForEyes[Eye::RIGHT] = dxm::Matrix::CreateTranslation({ -halfEyeDistance, 0.0f, 0.0f })
			* dxm::Matrix::CreatePerspectiveOffCenter(rightEyeL, rightEyeR, bottom, top, nearPlane, farPlane);
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

		auto camera = scene.ActiveCamera();
		auto frustum = Frustum(
			camera->EyePosition(), camera->Front(), camera->Right(),
			static_cast<float>(renderTarget->Width) / static_cast<float>(renderTarget->Height),
			FOV, NEAR_PLANE, FAR_PLANE);

		auto& context = m_Device->Context();

		ViewportDesc viewportDesc{ renderTarget->Width, renderTarget->Height };

		context->OMSetRenderTargets(1, renderTarget->RenderTargetView.GetAddressOf(), renderTarget->DepthStencilView.Get());
		context->RSSetViewports(1, &viewportDesc);

		float cleanColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
		context->ClearRenderTargetView(renderTarget->RenderTargetView.Get(), cleanColor);
		context->ClearDepthStencilView(renderTarget->DepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		for (auto [entity, renderable] : scene.GetAllEntitiesWith<IRenderable>())
		{
			auto id = entity;
			SceneEntity entity(scene, id);

			for (auto& renderingPass : renderable.RenderingPasses)
			{
				if (renderingPass.m_ShouldSkip)
				{
					continue;
				}

				auto& instancing = renderingPass.m_Instancing;

				auto& pipeline = renderingPass.m_Pipeline;
				auto& mesh = renderingPass.m_Mesh;

				if (!mesh.m_VertexBuffer || !mesh.m_IndexBuffer || !pipeline.m_VertexShader || !pipeline.m_PixelShader)
				{
					continue;
				}

				auto& aabbCallback = mesh.m_BoundingBoxCallback;

				if (mesh.m_BoundingBoxCallback.has_value())
				{
					auto& aabbCallback = mesh.m_BoundingBoxCallback.value();
					auto aabb = aabbCallback();

					if (!frustum.IsInside(aabb))
					{
						continue;
					}
				}

				if (instancing.has_value())
				{
					m_InstanceRenderQueue.Add(renderingPass);
					continue;
				}

				RenderIndividually(renderingPass, scene);
				m_DrawCallsThisFrame++;
			}
		}

		DeployInstancedQueue(scene);
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

	void AnaglyphRendererImpl::DeployInstancedQueue(Scene& scene)
	{
		m_InstanceRenderQueue.ForEach(
			[this, &scene](const RenderingPass& renderingPass, const InstanceRenderQueue::InstanceRawDataBuffer& rawInstanceData)
			{
				auto& pipeline = renderingPass.m_Pipeline;
				auto& mesh = renderingPass.m_Mesh;

				auto& vb = mesh.m_VertexBuffer;
				auto& ib = mesh.m_IndexBuffer;

				auto& il = mesh.m_InputLayout;

				m_InstanceBuffer->Update(rawInstanceData.Data(), rawInstanceData.Size());

				renderingPass.m_PreRenderCallback(renderingPass, scene);

				ID3D11Buffer* vertexBuffers[] = { vb->Get(), m_InstanceBuffer->Get() };
				UINT strides[] = { vb->Stride(), rawInstanceData.Size() / rawInstanceData.Count() };
				UINT offsets[] = { vb->Offset(), 0 };

				auto& context = m_Device->Context();

				context->IASetInputLayout(il->Layout());
				context->IASetVertexBuffers(0, 2, vertexBuffers, strides, offsets);
				context->IASetIndexBuffer(ib->Get(), ib->Format(), 0);
				context->IASetPrimitiveTopology(ib->Topology());

				SetupRasterizerState(renderingPass.m_RasterizerDescription);
				SetupShaderPipeline(renderingPass.m_Pipeline);

				context->DrawIndexedInstanced(ib->Count(), rawInstanceData.Count(), 0, 0, 0);

				m_DrawCallsThisFrame++;
				m_InstancesDrawnThisFrame += rawInstanceData.Count();

				CleanAfterDrawing();
			}
		);

		m_InstanceRenderQueue.Clear();
	}
}
