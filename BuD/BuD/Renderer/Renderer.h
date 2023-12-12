#pragma once

#include "GraphicsDevice.h"
#include "RenderTarget.h"
#include "Texture.h"

#include <Renderer/Implementations/BaseRendererImpl.h>
#include <Renderer/Implementations/MultiEyeRendererImpl.h>
#include <Renderer/Implementations/RenderingMode.h>

#include <Scene/Scene.h>
#include <Utils/Timepoint.h>

#include <map>
#include <stack>

namespace BuD
{
	class Renderer
	{
	public:
		static void Initialize(std::shared_ptr<Win32Window> window);
		static void Dispose();

		static void BeginTarget(int width, int height);
		static Texture EndTarget();

		static float AspectRatio();

		static void UseLeftHandCoordinateSystem();
		static void UseRightHandCoordinateSystem();

		static bool IsMultiEyeMode();

		static RenderingMode GetRenderingMode();
		static void SetRenderingMode(RenderingMode mode);

		static MultiEyeSettings GetMultiEyeSettings();
		static void SetMultiEyeSettings(const MultiEyeSettings& settings);

		static dxm::Matrix ProjectionMatrix();

		static void BeginFrame();
		static void Clear(float r, float g, float b, float a);
		static void Render(Scene& scene);
		static void EndFrame();

		inline static RendererFrameStats GetLastFrameStats() { return s_PerformanceData; }

		static void WindowResized(int width, int height);

		inline static std::shared_ptr<GraphicsDevice> Device() { return s_Device; }

	private:
		static void InitializeSwapchainBuffers(int width, int height);
		static void RenderToSwapchain();
		static void RenderToExternalTarget();

		inline static std::map<RenderingMode, std::shared_ptr<BaseRendererImpl>> s_RenderingImplementations = {};
		inline static std::set<std::shared_ptr<MultiEyeRendererImpl>> s_MultiEyeRendererImplementations = {};
		
		inline static std::shared_ptr<BaseRendererImpl> s_ActiveRendererImpl = nullptr;

		inline static RendererFrameStats s_PerformanceData = {};
		inline static RenderingMode s_RenderingMode = RenderingMode::STANDARD;
		inline static MultiEyeSettings s_MultiEyeSettings = {};

		// Device
		inline static std::shared_ptr<GraphicsDevice> s_Device = nullptr;

		// Swapchain related resources
		inline static ComPtr<ID3D11Texture2D> s_MainRTVTexture = nullptr;
		inline static ComPtr<ID3D11RenderTargetView> s_MainRTV = nullptr;
		inline static ComPtr<ID3D11DepthStencilView> s_DepthBuffer = nullptr;
		inline static bool s_RenderingToSwapchain = true;

		// Collection of render targets
		inline static std::stack<std::shared_ptr<RenderTarget>> s_ActiveRenderTargets = {};
		inline static std::vector<std::shared_ptr<RenderTarget>> s_FreeRenderTargets = {};
		inline static std::vector<std::shared_ptr<RenderTarget>> s_UsedRenderTargets = {};
	};
}
