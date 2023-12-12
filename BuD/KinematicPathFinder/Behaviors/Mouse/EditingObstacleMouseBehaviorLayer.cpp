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

	if (appStateDataLayer.m_AppState == AppState::ADD_OBSTACLE)
	{
		m_MainDataLayer.m_SceneDataLayer.AddNewObstacle();
	}
}

auto EditingObstacleMouseBehaviorLayer::OnMouseMove(int x, int y) -> void
{
	auto& viewportDataLayer = m_MainDataLayer.m_ViewportDataLayer;
	auto& sceneDataLayer = m_MainDataLayer.m_SceneDataLayer;

	if (!viewportDataLayer.IsMouseOnViewport(x, y))
	{
		return;
	}

	auto& obstacles = sceneDataLayer.m_ObstacleCollection;

	auto [xSS, ySS] = viewportDataLayer.MousePositionToScreenSpace(x, y);
	obstacles.ModifyNewObstacle({ xSS, ySS });
}

auto EditingObstacleMouseBehaviorLayer::HandleAddingObstacle(int x, int y) -> void
{
	auto& viewportDataLayer = m_MainDataLayer.m_ViewportDataLayer;
	auto& sceneDataLayer = m_MainDataLayer.m_SceneDataLayer;

	if (!viewportDataLayer.IsMouseOnViewport(x, y))
	{
		return;
	}

	auto [xSS, ySS] = viewportDataLayer.MousePositionToScreenSpace(x, y);

	auto& obstacles = sceneDataLayer.m_ObstacleCollection;
	obstacles.CreateNewObstacle({ xSS, ySS }, { xSS, ySS });
}
