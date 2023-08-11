#include "MainMouseBehaviorLayer.h"

#include <Behaviors/Mouse/Action/ActionMouseBehaviorLayer.h>
#include <Behaviors/Mouse/Selection/SelectionMouseBehaviorLayer.h>
#include <Behaviors/Mouse/Camera/CameraMouseBehaviorLayer.h>

MainMouseBehaviorLayer::MainMouseBehaviorLayer(MainDataLayer& dataLayer)
	: BaseMouseBehaviorLayer(dataLayer)
{
	m_BehaviorLayers.emplace_back(std::make_unique<ActionMouseBehaviorLayer>(dataLayer));
	m_BehaviorLayers.emplace_back(std::make_unique<SelectionMouseBehaviorLayer>(dataLayer));
	m_BehaviorLayers.emplace_back(std::make_unique<CameraMouseBehaviorLayer>(dataLayer));
}

void MainMouseBehaviorLayer::OnLeftButtonDown(int x, int y)
{
	for (auto& layer : m_BehaviorLayers)
	{
		layer->OnLeftButtonDown(x, y);
	}
}

void MainMouseBehaviorLayer::OnRightButtonDown(int x, int y)
{
	for (auto& layer : m_BehaviorLayers)
	{
		layer->OnRightButtonDown(x, y);
	}
}

void MainMouseBehaviorLayer::OnMiddleButtonDown(int x, int y)
{
	for (auto& layer : m_BehaviorLayers)
	{
		layer->OnMiddleButtonDown(x, y);
	}
}

void MainMouseBehaviorLayer::OnLeftButtonUp(int x, int y)
{
	for (auto& layer : m_BehaviorLayers)
	{
		layer->OnLeftButtonUp(x, y);
	}
}

void MainMouseBehaviorLayer::OnRightButtonUp(int x, int y)
{
	for (auto& layer : m_BehaviorLayers)
	{
		layer->OnRightButtonUp(x, y);
	}
}

void MainMouseBehaviorLayer::OnMiddleButtonUp(int x, int y)
{
	for (auto& layer : m_BehaviorLayers)
	{
		layer->OnMiddleButtonUp(x, y);
	}
}

void MainMouseBehaviorLayer::OnScroll(int x, int y, int delta)
{
	for (auto& layer : m_BehaviorLayers)
	{
		layer->OnScroll(x, y, delta);
	}
}

void MainMouseBehaviorLayer::OnMouseMove(int dx, int dy)
{
	for (auto& layer : m_BehaviorLayers)
	{
		layer->OnMouseMove(dx, dy);
	}
}
