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

	void EditableTexture::CopyFromBuffer(const std::vector<float>& buffer)
	{
		if (!m_EditingMode)
		{
			return;
		}

		if (m_Buffer.size() == buffer.size())
		{
			std::memcpy(m_Buffer.data(), buffer.data(), buffer.size() * sizeof(float));
		}
	}
	
	dxm::Vector4 EditableTexture::Sample(float x, float y)
	{
		if (!m_EditingMode)
		{
			return { NAN, NAN, NAN, NAN };
		}

		auto pixelCoords = ScreenSpaceToPixel(x, y);
		
		return Sample(pixelCoords.first, pixelCoords.second);
	}
	
	void EditableTexture::Clear(dxm::Vector4 color)
	{
		if (!m_EditingMode)
		{
			return;
		}

		auto dataStart = m_Buffer.data();

		std::for_each(std::execution::par, m_Buffer.begin(), m_Buffer.end(),
			[color, dataStart](float& in)
			{
				auto channel = (&in - dataStart) % 4;
				auto colorPtr = reinterpret_cast<float*>(const_cast<dxm::Vector4*>(&color));

				in = *(colorPtr + channel);
			}
		);
	}

	void EditableTexture::DrawLine(float x1, float y1, float x2, float y2, dxm::Vector4 color)
	{
		if (!m_EditingMode)
		{
			return;
		}

		auto [x1m, y1m] = ScreenSpaceToPixel(x1, y1);
		auto [x2m, y2m] = ScreenSpaceToPixel(x2, y2);
		
		auto dx = x2m - x1m;
		auto dy = y2m - y1m;
		auto D = 2.0f * dy - dx;

		auto y = y1;

		if (abs(dy) < abs(dx))
		{
			if (dx < 0.0f)
			{
				DrawLineLow(x2m, y2m, x1m, y1m, color);
			}
			else
			{
				DrawLineLow(x1m, y1m, x2m, y2m, color);
			}
		}
		else
		{
			if (dy < 0.0f)
			{
				DrawLineHigh(x2m, y2m, x1m, y1m, color);
			}
			else
			{
				DrawLineHigh(x1m, y1m, x2m, y2m, color);
			}
		}
	}
	
	void EditableTexture::FloodFill(float x, float y, dxm::Vector4 color)
	{
		if (!m_EditingMode)
		{
			return;
		}

		auto pixelCoords = ScreenSpaceToPixel(x, y);

		FloodFill(pixelCoords.first, pixelCoords.second, color);
	}

	void EditableTexture::DrawLineLow(int x1, int y1, int x2, int y2, dxm::Vector4 color)
	{
		auto dx = x2 - x1, dy = y2 - y1;
		auto yi = 1.0f;

		if (dy < 0)
		{
			yi = -1;
			dy = -dy;
		}

		auto D = 2.0f * dy - dx;
		auto y = y1;

		for (int x = x1; x <= x2; x++)
		{
			PutPixel(x, y, color);

			if (D > 0.0f)
			{
				y += yi;
				D -= 2.0f * dx;
			}

			D += 2.0f * dy;
		}
	}

	void EditableTexture::DrawLineHigh(int x1, int y1, int x2, int y2, dxm::Vector4 color)
	{
		auto dx = x2 - x1, dy = y2 - y1;
		auto xi = 1.0f;

		if (dx < 0)
		{
			xi = -1;
			dx = -dx;
		}

		auto D = 2.0f * dx - dy;
		auto x = x1;

		for (int y = y1; y <= y2; y++)
		{
			PutPixel(x, y, color);

			if (D > 0.0f)
			{
				x += xi;
				D -= 2.0f * dy;
			}

			D += 2.0f * dx;
		}
	}
	
	dxm::Vector4 EditableTexture::Sample(int x, int y)
	{
		auto index = static_cast<size_t>(4) * (m_Width * y + x);

		return dxm::Vector4(m_Buffer[index], m_Buffer[index + 1], m_Buffer[index + 2], m_Buffer[index + 3]);
	}

	void EditableTexture::FloodFill(int x, int y, dxm::Vector4 color)
	{
		if (x < 0 || x >= m_Width || y < 0 || y >= m_Height)
		{
			return;
		}

		std::queue<std::pair<int, int>> pointsToFill = {};
		pointsToFill.emplace(x, y);

		auto startingColor = Sample(x, y);

		while (!pointsToFill.empty())
		{
			auto [currX, currY] = pointsToFill.front();
			pointsToFill.pop();

			if (currX < 0 || currX >= m_Width || currY < 0 || currY >= m_Height)
			{
				continue;
			}

			auto sampledColor = Sample(currX, currY);

			if (sampledColor != startingColor)
			{
				continue;
			}

			PutPixel(currX, currY, color);

			pointsToFill.emplace(currX - 1, currY);
			pointsToFill.emplace(currX + 1, currY);
			pointsToFill.emplace(currX, currY - 1);
			pointsToFill.emplace(currX, currY + 1);
		}
	}

	std::pair<int, int> EditableTexture::ScreenSpaceToPixel(float x, float y)
	{
		if (m_Width == 0 || m_Height == 0)
		{
			return { 0, 0 };
		}

		auto xPixel = roundf(x < 0 ? 0 : (x >= 1.0f ? m_Width - 1 : x * (m_Width - 1)));
		auto yPixel = roundf(y < 0 ? 0 : (y >= 1.0f ? m_Height - 1 : y * (m_Height - 1)));

		return { static_cast<int>(xPixel), static_cast<int>(yPixel) };
	}

	void EditableTexture::PutPixel(int x, int y, dxm::Vector4 color)
	{
		if (!m_EditingMode || x < 0 || x >= m_Width || y < 0 || y >= m_Height)
		{
			return;
		}

		auto index = 4 * (m_Width * y + x);

		memcpy(&m_Buffer[index], &color, 4 * sizeof(float));
	}
}
