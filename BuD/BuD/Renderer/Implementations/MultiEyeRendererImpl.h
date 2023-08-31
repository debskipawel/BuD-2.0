#pragma once

#include <Renderer/Eye.h>
#include <Renderer/MultiEyeSettings.h>
#include <Renderer/Implementations/BaseRendererImpl.h>

namespace BuD
{
	class MultiEyeRendererImpl : public BaseRendererImpl
	{
	public:
		MultiEyeRendererImpl(std::shared_ptr<GraphicsDevice> device);

		virtual dxm::Matrix ProjectionMatrix() override;

		MultiEyeSettings GetMultiEyeSettings() const;
		void SetMultiEyeSettings(const MultiEyeSettings& settings);

	protected:

		virtual void UpdateProjectionMatrices(float aspectRatio, float fov, float nearPlane, float farPlane);

		Eye m_EyeForCurrentRendering;
		std::unordered_map<Eye, dxm::Matrix> m_ProjectionMatricesForEyes;

		std::unordered_map<Eye, std::unique_ptr<RenderTarget>> m_EyeRenderTargets;

		MultiEyeSettings m_MultiEyeSettings;
	};
}
