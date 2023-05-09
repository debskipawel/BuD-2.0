#pragma once

#include <d3d11.h>
#include <wrl/client.h>

#include <Scene/Scene.h>
#include <Renderer/RenderTarget.h>

using namespace Microsoft::WRL;

namespace BuD
{
	class BaseRendererImpl
	{
	public:
		virtual dxm::Matrix ProjectionMatrix() = 0;
		virtual void Render(Scene& scene, const RenderTargetInfo& renderTarget) = 0;
	};
}
