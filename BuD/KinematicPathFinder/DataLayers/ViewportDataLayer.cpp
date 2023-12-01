#include "ViewportDataLayer.h"

ViewportDataLayer::ViewportDataLayer()
	: m_Width(0), m_Height(0)
{
}

auto ViewportDataLayer::IsMouseOnViewport(int x, int y) -> bool
{
	return x >= m_ViewportMin.x && x <= m_ViewportMax.x && y >= m_ViewportMin.y && y <= m_ViewportMax.y;
}

auto ViewportDataLayer::MousePositionToScreenSpace(int x, int y) -> std::pair<float, float>
{
	auto screenSpaceX = 2.0f * (static_cast<float>(x) - m_ViewportMin.x) / (m_ViewportMax.x - m_ViewportMin.x) - 1.0f;
	auto screenSpaceY = 1.0f - 2.0f * (static_cast<float>(y) - m_ViewportMin.y) / (m_ViewportMax.y - m_ViewportMin.y);

	auto aspectRatio = static_cast<float>(m_Width) / static_cast<float>(m_Height);

	return { aspectRatio * screenSpaceX, screenSpaceY };
}
