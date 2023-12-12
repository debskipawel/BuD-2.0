#include "RobotConfigurationMouseBehaviorLayer.h"

RobotConfigurationMouseBehaviorLayer::RobotConfigurationMouseBehaviorLayer(MainDataLayer& mainDataLayer)
	: BaseMouseBehaviorLayer(mainDataLayer)
{
}

auto RobotConfigurationMouseBehaviorLayer::OnLeftButtonDown(int x, int y) -> void
{
	auto& viewportDataLayer = m_MainDataLayer.m_ViewportDataLayer;

	if (!viewportDataLayer.IsMouseOnViewport(x, y))
	{
		return;
	}

	auto appState = m_MainDataLayer.m_AppStateDataLayer.m_AppState;

	switch (appState)
	{
		case AppState::EDIT_START:
		{
			EditStartRobotConfiguration(x, y);
			break;
		}
		case AppState::EDIT_END:
		{
			EditEndRobotConfiguration(x, y);
			break;
		}
		default:
		{
			break;
		}
	}
}

auto RobotConfigurationMouseBehaviorLayer::EditStartRobotConfiguration(int x, int y) -> void
{
	auto [xSS, ySS] = m_MainDataLayer.m_ViewportDataLayer.MousePositionToScreenSpace(x, y);

	auto& sceneDataLayer = m_MainDataLayer.m_SceneDataLayer;

	sceneDataLayer.UpdateStartConfigurationPoints(m_MainDataLayer.m_SceneDataLayer.m_StartConfiguration.GetP0(), {xSS, ySS});
	
	sceneDataLayer.RedrawPathInRobotAngleParameterSpace();
}

auto RobotConfigurationMouseBehaviorLayer::EditEndRobotConfiguration(int x, int y) -> void
{
	auto [xSS, ySS] = m_MainDataLayer.m_ViewportDataLayer.MousePositionToScreenSpace(x, y);

	auto& sceneDataLayer = m_MainDataLayer.m_SceneDataLayer;

	sceneDataLayer.UpdateEndConfigurationPoints(m_MainDataLayer.m_SceneDataLayer.m_EndConfiguration.GetP0(), {xSS, ySS});

	sceneDataLayer.RecalculateRobotPathsInParameterSpace();
	sceneDataLayer.RedrawRobotAngleParameterSpace();
	sceneDataLayer.RedrawPathInRobotAngleParameterSpace();
}
