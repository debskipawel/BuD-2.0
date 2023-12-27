#include "MainMouseBehaviorLayer.h"

#include <Behaviors/Mouse/Camera/CameraMouseBehaviorLayer.h>

MainMouseBehaviorLayer::MainMouseBehaviorLayer(MainDataLayer& mainDataLayer)
	: BaseMouseBehaviorLayer(mainDataLayer)
{
	m_BehaviorLayers.emplace_back(std::make_unique<CameraMouseBehaviorLayer>(m_MainDataLayer));
}

auto MainMouseBehaviorLayer::OnLeftButtonDown(int x, int y) -> void
{
	for (const auto& behaviorLayer : m_BehaviorLayers)
	{
		behaviorLayer->OnLeftButtonDown(x, y);
	}
}

auto MainMouseBehaviorLayer::OnMiddleButtonDown(int x, int y) -> void
{
	for (const auto& behaviorLayer : m_BehaviorLayers)
	{
		behaviorLayer->OnMiddleButtonDown(x, y);
	}
}

auto MainMouseBehaviorLayer::OnRightButtonDown(int x, int y) -> void
{
	for (const auto& behaviorLayer : m_BehaviorLayers)
	{
		behaviorLayer->OnRightButtonDown(x, y);
	}
}

auto MainMouseBehaviorLayer::OnLeftButtonUp(int x, int y) -> void
{
	for (const auto& behaviorLayer : m_BehaviorLayers)
	{
		behaviorLayer->OnLeftButtonUp(x, y);
	}
}

auto MainMouseBehaviorLayer::OnMiddleButtonUp(int x, int y) -> void
{
	for (const auto& behaviorLayer : m_BehaviorLayers)
	{
		behaviorLayer->OnMiddleButtonUp(x, y);
	}
}

auto MainMouseBehaviorLayer::OnRightButtonUp(int x, int y) -> void
{
	for (const auto& behaviorLayer : m_BehaviorLayers)
	{
		behaviorLayer->OnRightButtonUp(x, y);
	}
}

auto MainMouseBehaviorLayer::OnMove(int x, int y, int dx, int dy) -> void
{
	for (const auto& behaviorLayer : m_BehaviorLayers)
	{
		behaviorLayer->OnMove(x, y, dx, dy);
	}
}

auto MainMouseBehaviorLayer::OnScroll(int x, int y, int delta) -> void
{
	for (const auto& behaviorLayer : m_BehaviorLayers)
	{
		behaviorLayer->OnScroll(x, y, delta);
	}
}
