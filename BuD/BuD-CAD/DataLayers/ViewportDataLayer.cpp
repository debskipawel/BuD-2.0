#include "ViewportDataLayer.h"

bool ViewportDataLayer::IsMouseOnViewport(int x, int y)
{
	auto viewportPosX = x - m_ViewportPosition.x;
	auto viewportPosY = y - m_ViewportPosition.y;

	return (viewportPosX >= 0 && viewportPosX < m_ViewportWidth)
		&& (viewportPosY >= 0 && viewportPosY < m_ViewportHeight);
}
