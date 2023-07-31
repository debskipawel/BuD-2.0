#pragma once

#include "BaseRendererImpl.h"

#include <map>
#include <vector>

#include <Buffers/InstanceBuffer.h>
#include <Renderer/InstanceRenderQueue/InstanceRenderQueue.h>
#include <Renderer/GraphicsDevice.h>
#include <Renderer/RenderingPass.h>

namespace BuD
{
	class StandardRendererImpl : public BaseRendererImpl
	{
	public:
		StandardRendererImpl(std::shared_ptr<GraphicsDevice> device);

		virtual dxm::Matrix ProjectionMatrix() override;
		virtual RendererFrameStats Render(Scene& scene, const RenderTargetInfo& renderTarget) override;

	protected:
		virtual void DeployInstancedQueue(Scene& scene);

		InstanceRenderQueue m_InstanceRenderQueue;
		std::shared_ptr<InstanceBuffer> m_InstanceBuffer;

		dxm::Matrix m_ProjectionMatrix;

		unsigned int m_DrawCallsThisFrame;
		unsigned int m_InstancesDrawnThisFrame;
	};
}
