#pragma once

#include "BaseRendererImpl.h"

#include <map>
#include <vector>

#include <Buffers/InstanceBuffer.h>

#include <Renderer/InstanceRenderQueue/InstanceRenderQueue.h>
#include <Renderer/GraphicsDevice.h>
#include <Renderer/RenderingPass.h>

#include <Renderer/Implementations/AnaglyphSettings.h>

namespace BuD
{
	class AnaglyphRendererImpl : public BaseRendererImpl
	{
	public:
		AnaglyphRendererImpl(std::shared_ptr<GraphicsDevice> device);

		virtual dxm::Matrix ProjectionMatrix() override;
		virtual RendererFrameStats Render(Scene& scene, const RenderTargetInfo& renderTarget) override;

	protected:
		enum Eye
		{
			LEFT = 0,
			RIGHT = 1,
		};

		virtual void UpdateProjectionMatrices(float aspectRatio, float fov, float nearPlane, float farPlane);
		virtual void UpdateEyeRenderTargets(unsigned int width, unsigned int height);
		
		virtual void RenderSceneForEye(Eye eye, Scene& scene);
		virtual void BlendEyesToSingle(const RenderTargetInfo& renderTarget);

		virtual void DeployInstancedQueue(Scene& scene);

		AnaglyphSettings m_AnaglyphSettings;

		InstanceRenderQueue m_InstanceRenderQueue;
		std::shared_ptr<InstanceBuffer> m_InstanceBuffer;

		Eye m_EyeForCurrentRendering;
		std::unordered_map<Eye, dxm::Matrix> m_ProjectionMatricesForEyes;
		
		std::unordered_map<Eye, std::unique_ptr<RenderTarget>> m_EyeRenderTargets;
		unsigned int m_PreviousWidth, m_PreviousHeight;

		unsigned int m_DrawCallsThisFrame;
		unsigned int m_InstancesDrawnThisFrame;

		MeshDetails m_BlendMeshDetails;
		ShaderPipeline m_BlendShaderPipeline;
		ComPtr<ID3D11SamplerState> m_BlendSamplerState;
	};
}
