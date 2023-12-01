#include "MainMouseBehaviorLayer.h"

#include <Behaviors/Mouse/EditingObstacleMouseBehaviorLayer.h>

MainMouseBehaviorLayer::MainMouseBehaviorLayer(MainDataLayer& mainDataLayer)
	: BaseMouseBehaviorLayer(mainDataLayer)
{
	m_BehaviorLayers.emplace_back(std::make_unique<EditingObstacleMouseBehaviorLayer>(m_MainDataLayer));
}

auto MainMouseBehaviorLayer::OnLeftButtonDown(int x, int y) -> void
{
	for (auto& behaviorLayer : m_BehaviorLayers)
	{
		behaviorLayer->OnLeftButtonDown(x, y);
	}
}

auto MainMouseBehaviorLayer::OnMiddleButtonDown(int x, int y) -> void
{
	for (auto& behaviorLayer : m_BehaviorLayers)
	{
		behaviorLayer->OnMiddleButtonDown(x, y);
	}
}

auto MainMouseBehaviorLayer::OnRightButtonDown(int x, int y) -> void
{
	for (auto& behaviorLayer : m_BehaviorLayers)
	{
		behaviorLayer->OnRightButtonDown(x, y);
	}
}

auto MainMouseBehaviorLayer::OnLeftButtonUp(int x, int y) -> void
{
	for (auto& behaviorLayer : m_BehaviorLayers)
	{
		behaviorLayer->OnLeftButtonUp(x, y);
	}
}

auto MainMouseBehaviorLayer::OnMiddleButtonUp(int x, int y) -> void
{
	for (auto& behaviorLayer : m_BehaviorLayers)
	{
		behaviorLayer->OnMiddleButtonUp(x, y);
	}
}

auto MainMouseBehaviorLayer::OnRightButtonUp(int x, int y) -> void
{
	for (auto& behaviorLayer : m_BehaviorLayers)
	{
		behaviorLayer->OnRightButtonUp(x, y);
	}
}

auto MainMouseBehaviorLayer::OnMouseMove(int x, int y) -> void
{
	for (auto& behaviorLayer : m_BehaviorLayers)
	{
		behaviorLayer->OnMouseMove(x, y);
	}
}
