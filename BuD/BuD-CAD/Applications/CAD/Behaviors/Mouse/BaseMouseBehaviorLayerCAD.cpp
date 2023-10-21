#include "BaseMouseBehaviorLayerCAD.h"

BaseMouseBehaviorLayerCAD::BaseMouseBehaviorLayerCAD(MainDataLayerCAD& dataLayer)
	: m_MainDataLayer(dataLayer)
{
}

dxm::Vector2 BaseMouseBehaviorLayerCAD::ViewportScreenSpaceCoords(int x, int y)
{
	dxm::Vector2 result = { -1.0f, -1.0f };

	auto& viewport = m_MainDataLayer.m_ViewportDataLayer;

	if (viewport.IsMouseOnViewport(x, y))
	{
		auto viewportPosX = x - viewport.m_ViewportPosition.x;
		auto viewportPosY = y - viewport.m_ViewportPosition.y;

		result.x = 2.0f * (float)(viewportPosX) / viewport.m_ViewportWidth - 1.0f;
		result.y = 1.0f - 2.0f * (float)(viewportPosY) / viewport.m_ViewportHeight;
	}

	return result;
}
