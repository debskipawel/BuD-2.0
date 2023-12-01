#include "EditingObstacleMouseBehaviorLayer.h"

#include <random>

EditingObstacleMouseBehaviorLayer::EditingObstacleMouseBehaviorLayer(MainDataLayer& mainDataLayer)
	: BaseMouseBehaviorLayer(mainDataLayer)
{
}

auto EditingObstacleMouseBehaviorLayer::OnLeftButtonDown(int x, int y) -> void
{
	auto& appStateDataLayer = m_MainDataLayer.m_AppStateDataLayer;
	
	if (appStateDataLayer.m_AppState != AppState::ADD_OBSTACLE)
	{
		return;
	}
	
	HandleAddingObstacle(x, y);
}

auto EditingObstacleMouseBehaviorLayer::OnLeftButtonUp(int x, int y) -> void
{
	auto& appStateDataLayer = m_MainDataLayer.m_AppStateDataLayer;
	auto& viewportDataLayer = m_MainDataLayer.m_ViewportDataLayer;

	if (!viewportDataLayer.IsMouseOnViewport(x, y))
	{
		return;
	}

	if (m_EditedObstacle)
	{
		if (appStateDataLayer.m_AppState == AppState::ADD_OBSTACLE)
		{
			m_MainDataLayer.m_SceneDataLayer.AddNewObstacle(m_EditedObstacle);
		}

		m_EditedObstacle = std::shared_ptr<Obstacle>();
	}
}

auto EditingObstacleMouseBehaviorLayer::OnMouseMove(int x, int y) -> void
{
	auto& appStateDataLayer = m_MainDataLayer.m_AppStateDataLayer;
	auto& viewportDataLayer = m_MainDataLayer.m_ViewportDataLayer;

	if (!viewportDataLayer.IsMouseOnViewport(x, y))
	{
		return;
	}

	if (m_EditedObstacle)
	{
		auto [xSS, ySS] = viewportDataLayer.MousePositionToScreenSpace(x, y);

		m_EditedObstacle->UpdateSecondVertex({ xSS, ySS });
	}
}

auto EditingObstacleMouseBehaviorLayer::HandleAddingObstacle(int x, int y) -> void
{
	auto& viewportDataLayer = m_MainDataLayer.m_ViewportDataLayer;

	if (!viewportDataLayer.IsMouseOnViewport(x, y))
	{
		return;
	}

	auto [xSS, ySS] = viewportDataLayer.MousePositionToScreenSpace(x, y);

	auto rand = BuD::Random();
	auto randomColor = dxm::Vector3(rand.Next(), rand.Next(), rand.Next());

	m_EditedObstacle = std::make_shared<Obstacle>(m_MainDataLayer.m_SceneDataLayer.m_Scene, dxm::Vector2(xSS, ySS), dxm::Vector2(xSS, ySS), randomColor);
}
