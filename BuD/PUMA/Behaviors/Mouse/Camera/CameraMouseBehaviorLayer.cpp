#include "CameraMouseBehaviorLayer.h"

CameraMouseBehaviorLayer::CameraMouseBehaviorLayer(MainDataLayer& mainDataLayer)
	: BaseMouseBehaviorLayer(mainDataLayer)
{
}

auto CameraMouseBehaviorLayer::OnMiddleButtonDown(int x, int y) -> void
{
	auto& simulations = m_MainDataLayer.m_Simulations;

	auto mouseNotOnAnyViewport = std::none_of(simulations.begin(), simulations.end(),
		[x, y](std::shared_ptr<SimulationDataLayer> simulation)
		{
			auto& viewport = simulation->ViewportDetails();

			return viewport.IsMouseOnViewport(x, y);
		}
	);

	if (mouseNotOnAnyViewport)
	{
		return;
	}

	m_MainDataLayer.m_CameraDataLayer.m_RotateCamera = true;
}

auto CameraMouseBehaviorLayer::OnMiddleButtonUp(int x, int y) -> void
{
	auto& simulations = m_MainDataLayer.m_Simulations;

	auto mouseNotOnAnyViewport = std::none_of(simulations.begin(), simulations.end(),
		[x, y](std::shared_ptr<SimulationDataLayer> simulation)
		{
			auto& viewport = simulation->ViewportDetails();

			return viewport.IsMouseOnViewport(x, y);
		}
	);

	if (mouseNotOnAnyViewport)
	{
		return;
	}

	m_MainDataLayer.m_CameraDataLayer.m_RotateCamera = false;
}

auto CameraMouseBehaviorLayer::OnMove(int x, int y, int dx, int dy) -> void
{
	if (!m_MainDataLayer.m_CameraDataLayer.m_RotateCamera)
	{
		return;
	}
	
	for (const auto& simulation : m_MainDataLayer.m_Simulations)
	{
		auto camera = simulation->Camera();

		camera->RotateCamera(0.01f * dx, 0.01f * dy);
	}
}

auto CameraMouseBehaviorLayer::OnScroll(int x, int y, int delta) -> void
{
	auto& simulations = m_MainDataLayer.m_Simulations;

	auto mouseNotOnAnyViewport = std::none_of(simulations.begin(), simulations.end(),
		[x, y](std::shared_ptr<SimulationDataLayer> simulation)
		{
			auto& viewport = simulation->ViewportDetails();

			return viewport.IsMouseOnViewport(x, y);
		}
	);

	if (mouseNotOnAnyViewport)
	{
		return;
	}

	for (const auto& simulation : m_MainDataLayer.m_Simulations)
	{
		auto camera = simulation->Camera();

		camera->Zoom(-0.01f * delta);
	}
}
