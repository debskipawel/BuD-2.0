#include "BaseMouseBehaviorLayer.h"

BaseMouseBehaviorLayer::BaseMouseBehaviorLayer(MainViewModel& viewModel)
    : m_ViewModel(viewModel)
{
}

dxm::Vector2 BaseMouseBehaviorLayer::ViewportScreenSpaceCoords(int x, int y)
{
	dxm::Vector2 result = { -1.0f, -1.0f };

	if (IsMouseOnViewport(x, y))
	{
		auto& viewport = m_ViewModel.m_ViewportViewModel;
		auto viewportPosX = x - viewport.m_ViewportPosition.x;
		auto viewportPosY = y - viewport.m_ViewportPosition.y;

		result.x = 2.0f * (float)(viewportPosX) / viewport.m_ViewportWidth - 1.0f;
		result.y = 1.0f - 2.0f * (float)(viewportPosY) / viewport.m_ViewportHeight;
	}

	return result;
}

bool BaseMouseBehaviorLayer::IsMouseOnViewport(int x, int y)
{
	auto& viewport = m_ViewModel.m_ViewportViewModel;
	auto viewportPosX = x - viewport.m_ViewportPosition.x;
	auto viewportPosY = y - viewport.m_ViewportPosition.y;

	return (viewportPosX >= 0 && viewportPosX < viewport.m_ViewportWidth)
		&& (viewportPosY >= 0 && viewportPosY < viewport.m_ViewportHeight);
}
