#include "MainMouseBehaviorLayerCAD.h"

#include <Applications/CAD/Behaviors/Mouse/Action/ActionMouseBehaviorLayer.h>
#include <Applications/CAD/Behaviors/Mouse/Selection/SelectionMouseBehaviorLayer.h>
#include <Applications/CAD/Behaviors/Mouse/Camera/CameraMouseBehaviorLayer.h>

MainMouseBehaviorLayerCAD::MainMouseBehaviorLayerCAD(MainDataLayer& dataLayer)
	: BaseMouseBehaviorLayerCAD(dataLayer)
{
	m_BehaviorLayers.emplace_back(std::make_unique<ActionMouseBehaviorLayer>(dataLayer));
	m_BehaviorLayers.emplace_back(std::make_unique<SelectionMouseBehaviorLayer>(dataLayer));
	m_BehaviorLayers.emplace_back(std::make_unique<CameraMouseBehaviorLayer>(dataLayer));
}

void MainMouseBehaviorLayerCAD::OnLeftButtonDown(int x, int y)
{
	for (auto& layer : m_BehaviorLayers)
	{
		layer->OnLeftButtonDown(x, y);
	}
}

void MainMouseBehaviorLayerCAD::OnRightButtonDown(int x, int y)
{
	for (auto& layer : m_BehaviorLayers)
	{
		layer->OnRightButtonDown(x, y);
	}
}

void MainMouseBehaviorLayerCAD::OnMiddleButtonDown(int x, int y)
{
	for (auto& layer : m_BehaviorLayers)
	{
		layer->OnMiddleButtonDown(x, y);
	}
}

void MainMouseBehaviorLayerCAD::OnLeftButtonUp(int x, int y)
{
	for (auto& layer : m_BehaviorLayers)
	{
		layer->OnLeftButtonUp(x, y);
	}
}

void MainMouseBehaviorLayerCAD::OnRightButtonUp(int x, int y)
{
	for (auto& layer : m_BehaviorLayers)
	{
		layer->OnRightButtonUp(x, y);
	}
}

void MainMouseBehaviorLayerCAD::OnMiddleButtonUp(int x, int y)
{
	for (auto& layer : m_BehaviorLayers)
	{
		layer->OnMiddleButtonUp(x, y);
	}
}

void MainMouseBehaviorLayerCAD::OnScroll(int x, int y, int delta)
{
	for (auto& layer : m_BehaviorLayers)
	{
		layer->OnScroll(x, y, delta);
	}
}

void MainMouseBehaviorLayerCAD::OnMouseMove(int dx, int dy)
{
	for (auto& layer : m_BehaviorLayers)
	{
		layer->OnMouseMove(dx, dy);
	}
}
