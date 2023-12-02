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

	auto points = SolveInverseKinematic(sceneDataLayer.m_P0, dxm::Vector2(xSS, ySS), sceneDataLayer.m_L1, sceneDataLayer.m_L2);

	if (!points.empty())
	{
		sceneDataLayer.m_P2 = { xSS, ySS };
		sceneDataLayer.m_P1 = points[0];

		sceneDataLayer.m_RobotArm->UpdateRobotPoints(sceneDataLayer.m_P0, sceneDataLayer.m_P1, sceneDataLayer.m_P2);
	}
}

auto RobotConfigurationMouseBehaviorLayer::EditEndRobotConfiguration(int x, int y) -> void
{
	auto [xSS, ySS] = m_MainDataLayer.m_ViewportDataLayer.MousePositionToScreenSpace(x, y);

	auto& sceneDataLayer = m_MainDataLayer.m_SceneDataLayer;

	auto points = SolveInverseKinematic(sceneDataLayer.m_P0, dxm::Vector2(xSS, ySS), sceneDataLayer.m_L1, sceneDataLayer.m_L2);

	if (!points.empty())
	{
		sceneDataLayer.m_P2 = { xSS, ySS };
		sceneDataLayer.m_P1 = points[0];

		sceneDataLayer.m_RobotArm->UpdateRobotPoints(sceneDataLayer.m_P0, sceneDataLayer.m_P1, sceneDataLayer.m_P2);
	}
}

auto RobotConfigurationMouseBehaviorLayer::SolveInverseKinematic(const dxm::Vector2& j0, const dxm::Vector2& j2, float L1, float L2) -> std::vector<dxm::Vector2>
{
	auto p0 = dxm::Vector2(0.0f, 0.0f);
	auto p2 = j2 - j0;

	auto delta = -p2.y * p2.y * (p2.x * p2.x + p2.y * p2.y - (L1 - L2) * (L1 - L2)) * (p2.x * p2.x + p2.y * p2.y - (L1 + L2) * (L1 + L2));

	if (delta < 0.0f)
	{
		return {};
	}

	if (delta < 1e-6f)
	{
		auto x = (p2.x * (L1 * L1 - L2 * L2 + p2.x * p2.x + p2.y * p2.y)) / (2.0f * (p2.x * p2.x + p2.y * p2.y));
		auto y = (L1 * L1 * p2.y * p2.y + p2.y * p2.y * (p2.x * p2.x + p2.y * p2.y - L2 * L2)) / (2.0f * p2.y * (p2.x * p2.x + p2.y * p2.y));

		return { { x + j0.x, y + j0.y } };
	}

	auto sqrtDelta = sqrtf(delta);

	auto x1 = (p2.x * (L1 * L1 - L2 * L2 + p2.x * p2.x + p2.y * p2.y) - sqrtDelta) / (2.0f * (p2.x * p2.x + p2.y * p2.y));
	auto y1 = (L1 * L1 * p2.y * p2.y + p2.y * p2.y * (p2.x * p2.x + p2.y * p2.y - L2 * L2) + p2.x * sqrtDelta) / (2.0f * p2.y * (p2.x * p2.x + p2.y * p2.y));

	auto x2 = (p2.x * (L1 * L1 - L2 * L2 + p2.x * p2.x + p2.y * p2.y) + sqrtDelta) / (2.0f * (p2.x * p2.x + p2.y * p2.y));
	auto y2 = (L1 * L1 * p2.y * p2.y + p2.y * p2.y * (p2.x * p2.x + p2.y * p2.y - L2 * L2) - p2.x * sqrtDelta) / (2.0f * p2.y * (p2.x * p2.x + p2.y * p2.y));

	return { { x1 + j0.x, y1 + j0.y }, { x2 + j0.x, y2 + j0.y } };
}
