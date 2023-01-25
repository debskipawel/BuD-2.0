#pragma once

#include "GraphicsDevice.h"
#include "RenderTarget.h"
#include "Texture.h"

#include "Implementations/BaseRendererImpl.h"
#include "Implementations/RenderingMode.h"

#include <Scene/Scene.h>

#include <map>

namespace BuD
{
	class Renderer
	{
	public:
		static void Initialize(std::shared_ptr<Win32Window> window);
		static void Dispose();

		static void BeginTarget(int width, int height);
		static Texture EndTarget();

		static dxm::Matrix ProjectionMatrix();

		static void BeginFrame();
		static void Clear(float r, float g, float b, float a);
		static void Render(Scene& scene);
		static void EndFrame();

		static void WindowResized(int width, int height);

		inline static std::shared_ptr<GraphicsDevice> Device() { return s_Device; }

	private:
		static void InitializeSwapchainBuffers(int width, int height);
		static void RenderToSwapchain();
		static void RenderToExternalTarget();

		inline static std::map<RenderingMode, std::shared_ptr<BaseRendererImpl>> s_RenderingImplementations = {};
		inline static std::shared_ptr<BaseRendererImpl> s_ActiveRendererImpl = nullptr;

		// Device
		inline static std::shared_ptr<GraphicsDevice> s_Device = nullptr;

		// Swapchain related resources
		inline static ComPtr<ID3D11Texture2D> s_MainRTVTexture = nullptr;
		inline static ComPtr<ID3D11RenderTargetView> s_MainRTV = nullptr;
		inline static ComPtr<ID3D11DepthStencilView> s_DepthBuffer = nullptr;
		inline static bool s_RenderingToSwapchain = true;

		// Collection of render targets
		inline static std::vector<std::shared_ptr<RenderTarget>> s_FreeRenderTargets = {};
		inline static std::vector<std::shared_ptr<RenderTarget>> s_UsedRenderTargets = {};
	};
}
