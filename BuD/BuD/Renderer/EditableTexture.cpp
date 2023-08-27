#include "bud_pch.h"
#include "EditableTexture.h"

#include <Renderer/Renderer.h>
#include <Renderer/Structures/Texture2DDesc.h>

namespace BuD
{
	EditableTexture::EditableTexture(uint32_t width, uint32_t height)
		: m_Width(width), m_Height(height), m_EditingMode(false)
	{
		Texture2DDesc mainTextureDesc(width, height);
		mainTextureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		mainTextureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		mainTextureDesc.Usage = D3D11_USAGE_DYNAMIC;
		mainTextureDesc.MipLevels = 1;

		Texture2DDesc readTextureDesc = mainTextureDesc;
		readTextureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		readTextureDesc.Usage = D3D11_USAGE_STAGING;
		readTextureDesc.BindFlags = 0;

		auto device = Renderer::Device();
		auto& raw = device->Device();

		HRESULT hr; 
		
		hr = raw->CreateTexture2D(&mainTextureDesc, nullptr, m_MainTexture.GetAddressOf());
		hr = raw->CreateTexture2D(&readTextureDesc, nullptr, m_ReadTexture.GetAddressOf());

		hr = raw->CreateShaderResourceView(m_MainTexture.Get(), nullptr, m_SRV.GetAddressOf());

		int a = 5;
	}

	void EditableTexture::BeginEdit()
	{
		if (m_EditingMode)
		{
			return;
		}

		m_EditingMode = true;

		auto device = Renderer::Device();
		auto& context = device->Context();

		context->CopyResource(m_ReadTexture.Get(), m_MainTexture.Get());

		D3D11_MAPPED_SUBRESOURCE mappedSubresource = {};

		auto hr = context->Map(m_ReadTexture.Get(), 0, D3D11_MAP_READ, 0, &mappedSubresource);

		if (FAILED(hr))
		{
			return;
		}

		auto rowSize = static_cast<size_t>(4) * m_Width;
		uint8_t* dataPtr = reinterpret_cast<uint8_t*>(mappedSubresource.pData);

		m_Buffer.resize(rowSize * m_Height);

		for (int i = 0; i < m_Height; i++)
		{
			memcpy(m_Buffer.data() + i * rowSize, dataPtr, rowSize * sizeof(float));

			dataPtr += mappedSubresource.RowPitch;
		}

		context->Unmap(m_ReadTexture.Get(), 0);
	}

	void EditableTexture::EndEdit()
	{
		if (!m_EditingMode)
		{
			return;
		}

		m_EditingMode = false;

		auto device = Renderer::Device();
		auto& context = device->Context();

		D3D11_MAPPED_SUBRESOURCE mappedSubresource = {};

		auto hr = context->Map(m_MainTexture.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);

		if (FAILED(hr))
		{
			return;
		}

		auto rowSize = static_cast<size_t>(4) * m_Width;
		uint8_t* dataPtr = reinterpret_cast<uint8_t*>(mappedSubresource.pData);

		m_Buffer.resize(mappedSubresource.DepthPitch);

		for (int i = 0; i < m_Height; i++)
		{
			memcpy(dataPtr, m_Buffer.data() + i * rowSize, rowSize * sizeof(float));

			dataPtr += mappedSubresource.RowPitch;
		}

		context->Unmap(m_MainTexture.Get(), 0);

		m_Buffer.clear();
	}
	
	dxm::Vector4 EditableTexture::Sample(float x, float y)
	{
		if (!m_EditingMode)
		{
			return { NAN, NAN, NAN, NAN };
		}

		auto pixelCoords = ScreenSpaceToPixel(x, y);
		auto index = 4 * (m_Width * pixelCoords.second + pixelCoords.first);

		return dxm::Vector4(
			static_cast<float>(m_Buffer[index]) / 255,
			static_cast<float>(m_Buffer[index + 1]) / 255,
			static_cast<float>(m_Buffer[index + 2]) / 255,
			static_cast<float>(m_Buffer[index + 3]) / 255
		);
	}
	
	void EditableTexture::Clear(dxm::Vector4 color)
	{
		if (!m_EditingMode)
		{
			return;
		}

		float mappedColor[4] = { std::clamp(color.x, 0.0f, 1.0f), std::clamp(color.y, 0.0f, 1.0f), std::clamp(color.z, 0.0f, 1.0f), std::clamp(color.w, 0.0f, 1.0f) };

		for (size_t i = 0; i < m_Buffer.size(); i++)
		{
			m_Buffer[i] = mappedColor[i % 4];
		}
	}

	void EditableTexture::DrawLine(float x1, float y1, float x2, float y2, dxm::Vector4 color)
	{
		if (!m_EditingMode)
		{
			return;
		}

		float mappedColor[4] = { std::clamp(color.x, 0.0f, 1.0f), std::clamp(color.y, 0.0f, 1.0f), std::clamp(color.z, 0.0f, 1.0f), std::clamp(color.w, 0.0f, 1.0f) };
	}
	
	void EditableTexture::FloodFill(float x, float y, dxm::Vector4 color)
	{
		if (!m_EditingMode)
		{
			return;
		}
	}
	
	std::pair<uint32_t, uint32_t> EditableTexture::ScreenSpaceToPixel(float x, float y)
	{
		if (m_Width == 0 || m_Height == 0 || x < 0.0f || x > 1.0f || y < 0.0f || y > 1.0f)
		{
			return { 0, 0 };
		}

		return { static_cast<uint32_t>(x * (m_Width - 1)), static_cast<uint32_t>(y * (m_Height - 1)) };
	}

	void EditableTexture::PutPixel(uint32_t x, uint32_t y, dxm::Vector4 color)
	{
		if (m_Buffer.empty())
		{
			return;
		}

		auto pixelCoords = ScreenSpaceToPixel(x, y);
		auto index = 4 * (m_Width * pixelCoords.second + pixelCoords.first);

		float mappedColor[4] = { std::clamp(color.x, 0.0f, 1.0f), std::clamp(color.y, 0.0f, 1.0f), std::clamp(color.z, 0.0f, 1.0f), std::clamp(color.w, 0.0f, 1.0f) };

		memcpy(&m_Buffer[index], mappedColor, 4 * sizeof(float));
	}
}
