#include "MainMouseBehaviorLayerCNC.h"

#include <Applications/CNC/Behaviors/Mouse/Camera/CameraMouseBehaviorLayerCNC.h>

MainMouseBehaviorLayerCNC::MainMouseBehaviorLayerCNC(MainDataLayerCNC& dataLayer)
	: BaseMouseBehaviorLayerCNC(dataLayer)
{
	m_MouseBehaviorLayers.emplace_back(std::make_unique<CameraMouseBehaviorLayerCNC>(m_MainDataLayer));
}

void MainMouseBehaviorLayerCNC::OnLeftButtonDown(int x, int y)
{
	for (const auto& behaviorLayer : m_MouseBehaviorLayers)
	{
		behaviorLayer->OnLeftButtonDown(x, y);
	}
}

void MainMouseBehaviorLayerCNC::OnRightButtonDown(int x, int y)
{
	for (const auto& behaviorLayer : m_MouseBehaviorLayers)
	{
		behaviorLayer->OnRightButtonDown(x, y);
	}
}

void MainMouseBehaviorLayerCNC::OnMiddleButtonDown(int x, int y)
{
	for (const auto& behaviorLayer : m_MouseBehaviorLayers)
	{
		behaviorLayer->OnMiddleButtonDown(x, y);
	}
}

void MainMouseBehaviorLayerCNC::OnLeftButtonUp(int x, int y)
{
	for (const auto& behaviorLayer : m_MouseBehaviorLayers)
	{
		behaviorLayer->OnLeftButtonUp(x, y);
	}
}

void MainMouseBehaviorLayerCNC::OnRightButtonUp(int x, int y)
{
	for (const auto& behaviorLayer : m_MouseBehaviorLayers)
	{
		behaviorLayer->OnRightButtonUp(x, y);
	}
}

void MainMouseBehaviorLayerCNC::OnMiddleButtonUp(int x, int y)
{
	for (const auto& behaviorLayer : m_MouseBehaviorLayers)
	{
		behaviorLayer->OnMiddleButtonUp(x, y);
	}
}

void MainMouseBehaviorLayerCNC::OnScroll(int x, int y, int delta)
{
	for (const auto& behaviorLayer : m_MouseBehaviorLayers)
	{
		behaviorLayer->OnScroll(x, y, delta);
	}
}

void MainMouseBehaviorLayerCNC::OnMouseMove(int dx, int dy)
{
	for (const auto& behaviorLayer : m_MouseBehaviorLayers)
	{
		behaviorLayer->OnMouseMove(dx, dy);
	}
}
