#pragma once

#include "GraphicsDevice.h"

#include <Scene/Scene.h>

namespace BuD
{
	class Renderer
	{
	public:
		static void Initialize(std::shared_ptr<Win32Window> window);
		static void Dispose();

		static void BeginFrame();
		static void Render(const Scene& scene);
		static void EndFrame();

		static void UpdateBuffersSize(int width, int height);

		inline static std::shared_ptr<GraphicsDevice> Device() { return s_Device; }

	private:
		static void InitializeBuffers(int width, int height);

		inline static std::shared_ptr<GraphicsDevice> s_Device = nullptr;

		inline static ComPtr<ID3D11Texture2D> s_MainRTVTexture = nullptr;
		inline static ComPtr<ID3D11RenderTargetView> s_MainRTV = nullptr;
		inline static ComPtr<ID3D11DepthStencilView> s_DepthBuffer = nullptr;
	};
}
