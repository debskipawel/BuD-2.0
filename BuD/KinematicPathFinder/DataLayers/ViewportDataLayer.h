#pragma once

#include <BuD.h>

struct ViewportDataLayer
{
	ViewportDataLayer();

	virtual auto IsMouseOnViewport(int x, int y) -> bool;
	virtual auto MousePositionToScreenSpace(int x, int y) -> std::pair<float, float>;

	int m_Width;
	int m_Height;

	dxm::Vector2 m_WindowPos;
	dxm::Vector2 m_ViewportMin, m_ViewportMax;

	BuD::Texture m_ViewportImage;
};
