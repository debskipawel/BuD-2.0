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

		dxm::Vector4 Sample(float x, float y);

		void Clear(dxm::Vector4 color);
		void DrawLine(float x1, float y1, float x2, float y2, dxm::Vector4 color);
		void FloodFill(float x, float y, dxm::Vector4 color);

	protected:
		std::pair<uint32_t, uint32_t> ScreenSpaceToPixel(float x, float y);
		void PutPixel(uint32_t x, uint32_t y, dxm::Vector4 color);

		ComPtr<ID3D11Texture2D> m_MainTexture;
		ComPtr<ID3D11Texture2D> m_ReadTexture;

		bool m_EditingMode;
		std::vector<float> m_Buffer;
		
		uint32_t m_Width, m_Height;
	};
}
