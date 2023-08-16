#include "bud_pch.h"
#include "StandardRendererImpl.h"

#include <Buffers/InstanceBuffer.h>
#include <Camera/Frustum.h>
#include <Objects/Interfaces/IRenderable.h>
#include <Renderer/Structures/ViewportDesc.h>

namespace BuD
{
	StandardRendererImpl::StandardRendererImpl(std::shared_ptr<GraphicsDevice> device)
		: BaseRendererImpl(device), m_DrawCallsThisFrame(), m_InstancesDrawnThisFrame()
	{
		m_InstanceBuffer = std::make_shared<InstanceBuffer>(64, nullptr);
	}

	dxm::Matrix StandardRendererImpl::ProjectionMatrix()
	{
		return m_ProjectionMatrix;
	}

	RendererFrameStats StandardRendererImpl::Render(Scene& scene, const RenderTargetInfo& renderTarget)
	{
		auto camera = scene.ActiveCamera();
		
		auto aspectRatio = static_cast<float>(renderTarget.Width) / renderTarget.Height;
		auto fov = 90.0f;
		auto nearDistance = 0.01f;
		auto farDistance = 100.0f;
		
		auto frustum = Frustum(camera->EyePosition(), camera->Front(), camera->Right(), aspectRatio, fov, nearDistance, farDistance);

		m_ProjectionMatrix = dxm::Matrix::CreatePerspectiveFieldOfView(
			DirectX::XMConvertToRadians(fov), 
			aspectRatio, nearDistance, farDistance
		);

		m_DrawCallsThisFrame = 0;
		m_InstancesDrawnThisFrame = 0;

		auto& context = m_Device->Context();

		ViewportDesc viewportDesc{ renderTarget.Width, renderTarget.Height };

		context->OMSetRenderTargets(1, renderTarget.RenderTargetView.GetAddressOf(), renderTarget.DepthStencilView.Get());
		context->RSSetViewports(1, &viewportDesc);

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

		auto performanceData = RendererFrameStats{};
		performanceData.m_DrawCalls = m_DrawCallsThisFrame;
		performanceData.m_InstancesDrawn = m_InstancesDrawnThisFrame;

		return performanceData;
	}

	void StandardRendererImpl::DeployInstancedQueue(Scene& scene)
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
