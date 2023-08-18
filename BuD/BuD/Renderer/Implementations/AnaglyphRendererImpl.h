#pragma once

#include <Renderer/Implementations/MultiEyeRendererImpl.h>

#include <map>
#include <vector>

#include <Buffers/InstanceBuffer.h>

#include <Renderer/InstanceRenderQueue/InstanceRenderQueue.h>
#include <Renderer/RenderingPass.h>

namespace BuD
{
	class AnaglyphRendererImpl : public MultiEyeRendererImpl
	{
	public:
		AnaglyphRendererImpl(std::shared_ptr<GraphicsDevice> device);

		virtual RendererFrameStats Render(Scene& scene, const RenderTargetInfo& renderTarget) override;

	protected:
		virtual void UpdateEyeRenderTargets(unsigned int width, unsigned int height);
		
		virtual void RenderSceneForEye(Eye eye, Scene& scene);
		virtual void BlendEyesToSingle(const RenderTargetInfo& renderTarget);

		std::unique_ptr<BaseRendererImpl> m_SingleEyeRendererImpl;
		unsigned int m_PreviousWidth, m_PreviousHeight;

		unsigned int m_DrawCallsThisFrame;
		unsigned int m_InstancesDrawnThisFrame;

		MeshDetails m_BlendMeshDetails;
		ShaderPipeline m_BlendShaderPipeline;
		ComPtr<ID3D11SamplerState> m_BlendSamplerState;
	};
}
