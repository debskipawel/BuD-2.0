#include "bud_pch.h"
#include "GraphicsDevice.h"

#include <DDSTextureLoader.h>
#include <WICTextureLoader.h>

#include <Utils/Log.h>

#include "Structures/SwapchainDesc.h"
#include "Structures/Texture2DDesc.h"

namespace BuD
{
	GraphicsDevice::GraphicsDevice(std::shared_ptr<Win32Window> window)
	{
		UpdateSize(window->Width(), window->Height());

		SwapchainDesc swapchainDesc(window->Handle(), m_Width, m_Height);
		UINT flags = 0;

#ifdef _DEBUG
		flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		auto hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, flags, nullptr, 0,
			D3D11_SDK_VERSION, &swapchainDesc, m_Swapchain.GetAddressOf(), m_Device.GetAddressOf(), nullptr, m_Context.GetAddressOf());

		if (FAILED(hr))
		{
			Log::WriteError(L"Error while creating a D3D11 device");
		}

		if (FAILED(CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED)))
		{
			Log::WriteWarning(L"Failed to initialize COM library, any texture load attempt will fail.");
		}
	}
	
	ComPtr<ID3D11RenderTargetView> GraphicsDevice::CreateRenderTargetView(const ComPtr<ID3D11Texture2D>& texture) const
	{
		ComPtr<ID3D11RenderTargetView> result;

		auto hr = m_Device->CreateRenderTargetView(texture.Get(), nullptr, result.GetAddressOf());

		if (FAILED(hr))
		{
			Log::WriteError(L"Error while creating a render target view");
		}

		return result;
	}
	
	ComPtr<ID3D11DepthStencilView> GraphicsDevice::CreateDepthStencilBuffer(UINT width, UINT height) const
	{
		auto textureDesc = Texture2DDesc::DepthStencilDescription(width, height);
		auto texture = CreateTexture(textureDesc);

		ComPtr<ID3D11DepthStencilView> result;
		auto hr = m_Device->CreateDepthStencilView(texture.Get(), nullptr, result.GetAddressOf());
		texture.Reset();

		if (FAILED(hr))
		{
			Log::WriteError(L"Error while creating depth buffer\n");
		}

		return result;
	}
	
	ComPtr<ID3D11Texture2D> GraphicsDevice::CreateTexture(const D3D11_TEXTURE2D_DESC& desc) const
	{
		ComPtr<ID3D11Texture2D> result;
		auto hr = m_Device->CreateTexture2D(&desc, nullptr, result.GetAddressOf());

		if (FAILED(hr))
		{
			Log::WriteError(L"Error while creating a texture");
		}

		return result;
	}

	ComPtr<ID3D11ShaderResourceView> GraphicsDevice::CreateShaderResourceView(std::filesystem::path filepath) const
	{
		HRESULT hr;
		ComPtr<ID3D11ShaderResourceView> srv;
		auto wPath = filepath.wstring();

		if (filepath.extension() == ".dds")
		{
			hr = DirectX::CreateDDSTextureFromFile(m_Device.Get(), m_Context.Get(), wPath.c_str(), nullptr, srv.GetAddressOf());
		}
		else
		{
			hr = DirectX::CreateWICTextureFromFile(m_Device.Get(), m_Context.Get(), wPath.c_str(), nullptr, srv.GetAddressOf());
		}

		if (FAILED(hr))
		{
			Log::WriteError(L"Error while creating SRV from file (check logs to see if COM library was properly initialized)");
		}

		return srv;
	}
	
	void GraphicsDevice::UpdateSize(uint32_t width, uint32_t height)
	{
		m_Width = width;
		m_Height = height;
	}
}
