#pragma once

#include <SimpleMath.h>
#include <Renderer/Texture.h>

namespace dxm = DirectX::SimpleMath;

namespace BuD
{
	class EditableTexture : public Texture
	{
	public:
		EditableTexture(uint32_t width, uint32_t height);

		void BeginEdit();
		void EndEdit();

		void CopyFromBuffer(const std::vector<float>& buffer);
		const std::vector<float>& GetRawBuffer();

		dxm::Vector4 Sample(float x, float y);

		void Clear(dxm::Vector4 color);
		void DrawLine(float x1, float y1, float x2, float y2, dxm::Vector4 color);
		void FloodFill(float x, float y, dxm::Vector4 color);

		dxm::Vector4 Sample(int x, int y);
		void PutPixel(int x, int y, dxm::Vector4 color);

		virtual uint32_t Width() const { return m_Width; }
		virtual uint32_t Height() const { return m_Height; }

	protected:
		void DrawLineLow(int x1, int y1, int x2, int y2, dxm::Vector4 color);
		void DrawLineHigh(int x1, int y1, int x2, int y2, dxm::Vector4 color);

		void FloodFill(int x, int y, dxm::Vector4 color);

		std::pair<int, int> ScreenSpaceToPixel(float x, float y);

		ComPtr<ID3D11Texture2D> m_MainTexture;
		ComPtr<ID3D11Texture2D> m_ReadTexture;

		bool m_EditingMode;
		std::vector<float> m_Buffer;
		
		uint32_t m_Width, m_Height;
	};
}
