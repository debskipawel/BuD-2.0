#include "bud_pch.h"
#include "StandardRendererImpl.h"

#include <Buffers/InstanceBuffer.h>
#include <Camera/Frustum.h>
#include <Objects/Interfaces/IRenderable.h>
#include <Renderer/Structures/ViewportDesc.h>

namespace BuD
{
	StandardRendererImpl::StandardRendererImpl(std::shared_ptr<GraphicsDevice> device)
		: m_Device(device), m_DrawCallsThisFrame(), m_InstancesDrawnThisFrame()
	{
	}

	dxm::Matrix StandardRendererImpl::ProjectionMatrix()
	{
		return m_ProjectionMatrix;
	}

	FramePerformanceData StandardRendererImpl::Render(Scene& scene, const RenderTargetInfo& renderTarget)
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

		auto startTime = Clock::Now();

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
				auto& instancing = renderingPass.m_Instancing;

				auto& pipeline = renderingPass.m_Pipeline;
				auto& mesh = renderingPass.m_Mesh;

				auto& aabbCallback = mesh.m_BoundingBoxCallback;

				if (mesh.m_BoundingBoxCallback.has_value())
				{
					auto& aabbCallback = mesh.m_BoundingBoxCallback.value();
					auto aabb = aabbCallback();

					if (frustum.IsInside(aabb))
					{
						Log::WriteInfo(L"Object outside view frustum, culling.");
						continue;
					}
				}

				if (instancing.has_value())
				{
					AddInstancedRenderToQueue(renderingPass);
					continue;
				}

				RenderIndividually(renderingPass, scene);
			}
		}

		DeployInstancedQueue(scene);
		ClearRenderQueue();

		auto endTime = Clock::Now();

		auto performanceData = FramePerformanceData{};
		performanceData.m_DrawCalls = m_DrawCallsThisFrame;
		performanceData.m_InstancesDrawn = m_InstancesDrawnThisFrame;
		performanceData.m_FrameTime = 1000.0f * (endTime - startTime);
		performanceData.m_FrameRate = 1000.0f / performanceData.m_FrameTime;

		return performanceData;
	}

	void StandardRendererImpl::RenderIndividually(const RenderingPass& renderingPass, Scene& scene)
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
			return;
		}

		renderingPass.m_PreRenderCallback(renderingPass, scene);

		auto rawVertexBuffer = vb->Get();
		const auto stride = vb->Stride();
		const auto offset = vb->Offset();

		auto& context = m_Device->Context();

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
		
		m_DrawCallsThisFrame++;

		ID3D11ShaderResourceView* clean[] = { nullptr };

		context->VSSetShaderResources(0, 1, clean);
		context->PSSetShaderResources(0, 1, clean);
	}

	void StandardRendererImpl::ClearRenderQueue()
	{
		m_InstanceRenderIndices.clear();
		m_InstanceRenderingData.clear();
	}

	void StandardRendererImpl::AddInstancedRenderToQueue(const RenderingPass& renderingPass)
	{
		auto& instancing = renderingPass.m_Instancing.value();

		unsigned int index;
		auto instancingIndexIter = m_InstanceRenderIndices.find(renderingPass);

		if (instancingIndexIter == m_InstanceRenderIndices.end())
		{
			index = m_InstanceRenderIndices.size();
			
			m_InstanceRenderIndices.insert(std::make_pair(renderingPass, index));
			m_InstanceRenderingData.push_back({});
		}
		else
		{
			index = instancingIndexIter->second;
		}

		auto& rawData = m_InstanceRenderingData[index];
		auto instanceData = instancing.m_InstanceCallback();

		auto currentSize = rawData.m_Data.size();

		// TODO: potential performance problems
		rawData.m_Data.resize(currentSize + instanceData.m_Size);
		
		std::memcpy(rawData.m_Data.data() + currentSize, instanceData.m_Data, instanceData.m_Size);
		rawData.m_InstanceCount++;
	}

	void StandardRendererImpl::DeployInstancedQueue(Scene& scene)
	{
		for (auto& [renderingPass, index] : m_InstanceRenderIndices)
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

			// TODO: no need to create it every time, one buffer can be stored and extended as needed
			auto& rawInstanceData = m_InstanceRenderingData[index];
			auto instanceBuffer = std::make_shared<InstanceBuffer>(rawInstanceData.m_Data.size(), rawInstanceData.m_Data.data());

			renderingPass.m_PreRenderCallback(renderingPass, scene);

			ID3D11Buffer* vertexBuffers[] = { vb->Get(), instanceBuffer->Get() };
			UINT strides[] = { vb->Stride(), rawInstanceData.m_Data.size() / rawInstanceData.m_InstanceCount };
			UINT offsets[] = { vb->Offset(), 0 };

			auto& context = m_Device->Context();

			context->IASetInputLayout(il->Layout());
			context->IASetVertexBuffers(0, 2, vertexBuffers, strides, offsets);
			context->IASetIndexBuffer(ib->Get(), ib->Format(), 0);
			context->IASetPrimitiveTopology(ib->Topology());

			auto vsCB = vs->RawConstantBuffers();
			context->VSSetConstantBuffers(0, vsCB.size(), vsCB.data());
			context->VSSetShader(vs->Shader(), nullptr, 0);

			auto psCB = ps->RawConstantBuffers();
			context->PSSetConstantBuffers(0, psCB.size(), psCB.data());
			context->PSSetShader(ps->Shader(), nullptr, 0);

			context->DrawIndexedInstanced(ib->Count(), rawInstanceData.m_InstanceCount, 0, 0, 0);

			m_DrawCallsThisFrame++;
			m_InstancesDrawnThisFrame += rawInstanceData.m_InstanceCount;

			ID3D11ShaderResourceView* clean[] = { nullptr };

			context->VSSetShaderResources(0, 1, clean);
			context->PSSetShaderResources(0, 1, clean);
		}
	}
}
