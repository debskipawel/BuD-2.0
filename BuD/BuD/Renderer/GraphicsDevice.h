#pragma once

#include <cstdint>
#include <d3d11.h>
#include <filesystem>
#include <wrl/client.h>

#include <Window/Win32Window.h>

using namespace Microsoft::WRL;

namespace BuD
{
	class GraphicsDevice
	{
	public:
		explicit GraphicsDevice(std::shared_ptr<Win32Window> window);
		
		GraphicsDevice(const GraphicsDevice&) = delete;
		GraphicsDevice& operator=(const GraphicsDevice& other) = delete;
		GraphicsDevice(GraphicsDevice&& other) = delete;

		inline const ComPtr<ID3D11Device>&			Device() const		{ return m_Device; }
		inline const ComPtr<ID3D11DeviceContext>&	Context() const		{ return m_Context; }
		inline const ComPtr<IDXGISwapChain>&		Swapchain() const	{ return m_Swapchain; }

		inline UINT BufferWidth() const		{ return m_Width; }
		inline UINT BufferHeight() const	{ return m_Height; }

		ComPtr<ID3D11RenderTargetView>		CreateRenderTargetView(const ComPtr<ID3D11Texture2D>& texture) const;
		ComPtr<ID3D11DepthStencilView>		CreateDepthStencilBuffer(UINT width, UINT height) const;
		ComPtr<ID3D11Texture2D>				CreateTexture(const D3D11_TEXTURE2D_DESC& desc) const;
		ComPtr<ID3D11ShaderResourceView>	CreateShaderResourceView(std::filesystem::path filepath) const;
		
		void UpdateSize(uint32_t width, uint32_t height);

	private:
		uint32_t m_Width, m_Height;

		ComPtr<ID3D11Device>		m_Device;
		ComPtr<ID3D11DeviceContext> m_Context;
		ComPtr<IDXGISwapChain>		m_Swapchain;
	};
}
