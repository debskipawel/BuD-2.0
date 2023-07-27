#pragma once

#include "BaseRendererImpl.h"

#include <map>
#include <vector>

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

		virtual void RenderIndividually(const RenderingPass& renderingPass, Scene& scene);
		
		virtual void ClearRenderQueue();

		virtual void AddInstancedRenderToQueue(const RenderingPass& renderingPass);
		virtual void DeployInstancedQueue(Scene& scene);

		struct InstanceRawData
		{
			std::vector<uint8_t> m_Data;
			unsigned int m_InstanceCount;
		};

		std::map<RenderingPass, unsigned int> m_InstanceRenderIndices;
		std::vector<InstanceRawData> m_InstanceRenderingData;

		std::shared_ptr<GraphicsDevice> m_Device;
		dxm::Matrix m_ProjectionMatrix;

		unsigned int m_DrawCallsThisFrame;
		unsigned int m_InstancesDrawnThisFrame;
	};
}
